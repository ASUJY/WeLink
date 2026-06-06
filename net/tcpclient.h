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
    explicit TcpClient(CommunicationMediator* mediator, QObject *parent = nullptr);
    ~TcpClient();

    bool OpenNetwork() override;
    void CloseNetwork() override;
    bool SendData(const QByteArray& data, int len) override;

    bool AttemptReconnect();


private:
    QTcpSocket* m_tcpSocket = nullptr;
    QTimer *m_retryTime = nullptr;
    int m_retryCount = 0;
};

}   // namespace net end

#endif // TCPCLIENT_H
