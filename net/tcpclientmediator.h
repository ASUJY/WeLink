#ifndef TCPCLIENTMEDIATOR_H
#define TCPCLIENTMEDIATOR_H

#include "communicationmediator.h"

namespace net {
class TcpClientMediator : public CommunicationMediator
{
    Q_OBJECT
public:
    TcpClientMediator();
    ~TcpClientMediator();

    bool SendData(const QByteArray& data, int len) override;
};

}   // namespace net end

#endif // TCPCLIENTMEDIATOR_H
