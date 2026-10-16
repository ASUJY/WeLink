#include "tcpclient.h"

#include <QNetworkProxy>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

namespace net {
const int MAXRETRIES = 3;

TcpClient::TcpClient(QObject *parent) :
    AbstractNet(parent),
    m_tcpSocket(new QTcpSocket(this)),
    m_retryTime(new QTimer(this))
{
    m_retryTime->setSingleShot(true);
    m_retryTime->setInterval(10000);

    connect(m_tcpSocket, &QTcpSocket::connected, this, [this]() {
        m_retryCount = 0;
        m_retryTime->stop();
        qInfo() << "TCP connection establised, successfully connected to the server.";
    });

    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this](){
        qWarning() << "TCP disconnected.";
        ScheduleReconnect();
    });

    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        qWarning() << "TCP Error: " << m_tcpSocket->errorString();
        qWarning() << "state: " << m_tcpSocket->state();
        //连接失败后，异步 socket 通常会回到 UnconnectedState。此时安排下一次尝试。
        if (m_networkEnabled && m_tcpSocket->state() == QAbstractSocket::UnconnectedState) {
            ScheduleReconnect();
        }
    });

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::RecvData);

    connect(m_retryTime, &QTimer::timeout, this, &TcpClient::AttemptReconnect);

    // 无代理,直连模式
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::NoProxy);
    QNetworkProxy::setApplicationProxy(proxy);
}

TcpClient::~TcpClient() {
    CloseNetwork();
}

void TcpClient::CloseNetwork() {
    // 显式关闭网络后，不再自动重连
    m_networkEnabled = false;
    if (m_retryTime) {
        m_retryTime->stop();
    }

    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost();
    }
}

void TcpClient::OpenNetwork() {
    m_networkEnabled = true;

    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState ||
        m_tcpSocket->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    m_retryCount = 0;
    AttemptReconnect();
}

void TcpClient::ScheduleReconnect() {
    if (!m_networkEnabled || !m_retryTime) {
        return;
    }

    if (m_retryCount >= MAXRETRIES) {
        qDebug() << "重连次数耗尽，连接失败";
        return;
    }

    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState ||
        m_tcpSocket->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    // 启动断线重连
    if (!m_retryTime->isActive()) {
        m_retryTime->start();
    }
}

void TcpClient::AttemptReconnect() {
    if (!m_networkEnabled || !m_tcpSocket) {
        return;
    }

    if (m_tcpSocket->state() != QAbstractSocket::UnconnectedState) {
        return;
    }

    QSettings conf("./init.conf", QSettings::IniFormat);
    const QString ip = conf.value("/SERVER/IP").toString();
    const int port = conf.value("/SERVER/PORT").toInt();

    if (ip.isEmpty() || port <= 0 || port > 65535) {
        qWarning() << "Invalid servet IP or port configuration.";
        ++m_retryCount;
        ScheduleReconnect();
        return;
    }

    ++m_retryCount;
    qInfo() << "First attempt to connect to：" << ip << ":" << port;

    m_tcpSocket->connectToHost(QHostAddress(ip), port);

    // 成功、失败由 connected/errorOccurred 信号处理。
}

bool TcpClient::SendData(const QByteArray& data, int len) {
    constexpr quint32 kMaxMessageLen = 1024 * 1024; // 1MB

    if (data.isEmpty() || len <= 0 || len != data.size()
        || static_cast<quint32>(len) > kMaxMessageLen) {
        return false;
    }

    if (!m_tcpSocket || m_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        // 后台会继续重连，但本次数据不会自动重发。
        return false;
    }

    // 将消息长度转换为网络字节序(大端序)
    quint32 netLen = qToBigEndian<quint32>(static_cast<quint32>(len));
    // 组装完整的数据帧: 4字节长度头+JSON消息体
    QByteArray packet;
    packet.resize(sizeof(netLen));
    std::memcpy(packet.data(), &netLen, sizeof(netLen));
    packet.append(data);

    if(m_tcpSocket->write(packet) != packet.size()) {
        return false;
    }

    // write() 成功表示数据被接受到发送缓冲区，并不表示服务器已经收到或处理。
    // 后续再处理这个问题
    return true;
}

void TcpClient::RecvData() {
    if (!m_tcpSocket) {
        return;
    }

    const QByteArray data = m_tcpSocket->readAll();
    if (!data.isEmpty()) {
        m_recvBuffer.append(data);
    }

    constexpr qsizetype kHeaderLen = 4;
    constexpr quint32 kMaxMessageLen = 1024 * 1024; // 1MB

    while (m_recvBuffer.size() >= kHeaderLen) {
        const auto* header = reinterpret_cast<const uchar*>(m_recvBuffer.constData());
        const quint32 bodyLen = qFromBigEndian<quint32>(header);

        if (bodyLen == 0 || bodyLen > kMaxMessageLen) {
            qWarning() << "Invalid message length:" << bodyLen;

            // 协议帧不合法，无法可靠恢复边界
            m_recvBuffer.clear();
            return;
        }

        const qsizetype totalLen = kHeaderLen + static_cast<qsizetype>(bodyLen);
        if (m_recvBuffer.size() < totalLen) {
            break;
        }

        const QByteArray body = m_recvBuffer.mid(kHeaderLen, static_cast<qsizetype>(bodyLen));
        m_recvBuffer.remove(0, totalLen);

        // 向上层发送完整 JSON 消息
        emit SIG_ReadyData(body, body.size());
    }
}

}   // namespace net end