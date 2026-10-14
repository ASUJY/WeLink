#include "tcpclientmediator.h"
#include "tcpclient.h"

namespace net {

TcpClientMediator::TcpClientMediator() {
    m_net = new TcpClient(this);
    connect(m_net, &AbstractNet::SIG_ReadyData, this, &CommunicationMediator::RecvData);
    m_net->OpenNetwork();
}

TcpClientMediator::~TcpClientMediator() {
}


bool TcpClientMediator::SendData(const QByteArray& data, int len) {
    return m_net && m_net->SendData(data, len);
}

void TcpClientMediator::RecvData(const QByteArray& data, int len) {
    emit SIG_ReadyData(data, len);
}

}   // namespace net end