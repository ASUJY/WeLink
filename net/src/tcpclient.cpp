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
    // 显示关闭网络后，不再自动重连
    m_networkEnabled = false;
    if (m_retryTime) {
        m_retryTime->stop();
    }

    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost();
    }


    // m_isOpenNetwork = false;
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
    if (data.isEmpty() || len <= 0) {
        return false;
    }

    if (!m_tcpSocket || m_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        // 后台会继续重连，但本次数据不会自动重发。
        return false;
    }

    if(m_tcpSocket->write(data) != len) {
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
    QByteArray data = m_tcpSocket->readAll();
    if (!data.isEmpty()) {
        emit SIG_ReadyData(data, data.size());
    }
}

}   // namespace net end