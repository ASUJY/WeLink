#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "abstractnet.h"

#include <QTcpSocket>
#include <QTimer>

namespace net {
class TcpClient : public AbstractNet
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    void OpenNetwork() override;
    void CloseNetwork() override;
    bool SendData(const QByteArray& data, int len) override;

public slots:
    void RecvData() override;

    void AttemptReconnect();

private:
    void ScheduleReconnect();

private:
    QTcpSocket* m_tcpSocket = nullptr;
    QByteArray m_recvBuffer;    // 累积 TCP 接收到的数据

    QTimer *m_retryTime = nullptr;
    int m_retryCount = 0;
    bool m_networkEnabled = false;
};

}   // namespace net end

#endif // TCPCLIENT_H