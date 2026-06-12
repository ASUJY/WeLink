#include "tcpclient.h"

#include <QNetworkProxy>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

namespace net {
const int MAXRETRIES = 3;

TcpClient::TcpClient(CommunicationMediator* mediator, QObject *parent) : AbstractNet(parent) {
    m_retryTime = new QTimer(this);
    m_commMediator = mediator;
}

TcpClient::~TcpClient() {
    CloseNetwork();
}

void TcpClient::CloseNetwork() {
    if (m_tcpSocket) {
        if (m_tcpSocket->isOpen()) {
            m_tcpSocket->close();
        }
        m_tcpSocket->disconnectFromHost();
        if (m_tcpSocket->state() != QAbstractSocket::UnconnectedState) {
            m_tcpSocket->waitForDisconnected();
        }
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }

    if (m_retryTime) {
        m_retryTime->stop();
        m_retryTime->deleteLater();
        m_retryTime = nullptr;
    }
    // m_isOpenNetwork = false;
}

bool TcpClient::OpenNetwork() {
    // if (m_isOpenNetwork) {
    //     return true;
    // }

    // 无代理,直连模式
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::NoProxy);
    QNetworkProxy::setApplicationProxy(proxy);

    if (m_tcpSocket == nullptr) {
        m_tcpSocket = new QTcpSocket(this);
    }

    // connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::SlotRecvFromSocket,  Qt::UniqueConnection);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::RecvData, Qt::UniqueConnection);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError err) {
        qDebug() << "连接错误：" << m_tcpSocket->errorString(); // 打印为什么连不上！
    });
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this]() {
        // m_isOpenNetwork = false;
        // 自动触发重连
        if (m_retryTime && !m_retryTime->isActive()) {
            m_retryTime->start();
        }
    });
    connect(m_retryTime, &QTimer::timeout, this, &TcpClient::AttemptReconnect,  Qt::UniqueConnection);

    m_retryTime->setInterval(10000);
    m_retryTime->setSingleShot(true);
    return AttemptReconnect();
}

bool TcpClient::AttemptReconnect() {
    if (m_retryCount >= MAXRETRIES) {
        m_retryTime->stop();
        m_retryCount = 0;
        qDebug() << "重连次数耗尽，连接失败";
        // m_networkMediator->ReportNetworkStatus();
        return false;
    }

    QSettings conf = QSettings("./init.conf", QSettings::IniFormat);
    QString ip = conf.value("/SERVER/IP").toString();
    int port = conf.value("/SERVER/PORT").toInt();

    qDebug() << "尝试连接：" << ip << port;

    if (m_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        // m_networkMediator->ReportNetworkStatus();
        m_tcpSocket->connectToHost(QHostAddress(ip), port);
        m_retryCount++;
        if (m_tcpSocket->waitForConnected(5000)) {
            qDebug() << "连接成功！";
            m_retryCount = 0;
            // m_isOpenNetwork = true;
            m_retryTime->stop();
            return true;
        } else {
            qDebug() << "连接失败，错误信息：" << m_tcpSocket->errorString();
            // ========== 修复：失败后启动定时器，自动重连 ==========
            m_retryTime->start();
        }
        return false;
    }
    m_retryTime->stop();
    // m_isOpenNetwork = true;
    return true;
}

bool TcpClient::SendData(const QByteArray& data, int len) {
    if (!OpenNetwork()) {
        return false;
    }
    if (data.isEmpty() || len < 0) {
        return false;
    }

    if (m_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        return false;
    }

    if(m_tcpSocket->write(data) != len) {
        return false;
    }

    if (!m_tcpSocket->waitForBytesWritten(3000)) {
        return false;
    }
    return true;
}

void TcpClient::RecvData() {
    qDebug() << "TcpClient";
    QByteArray data = m_tcpSocket->readAll();
    m_commMediator->RecvData(data, data.size());
}

}   // namespace net end