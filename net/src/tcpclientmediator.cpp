#include "tcpclientmediator.h"
#include "tcpclient.h"

namespace net {

TcpClientMediator::TcpClientMediator() {
    m_net = new TcpClient(this);
    connect(m_net, &AbstractNet::SIG_ReadyData, this, &CommunicationMediator::RecvData);
}

TcpClientMediator::~TcpClientMediator() {
    if (m_net) {
        delete m_net;
        m_net = nullptr;
    }
}


bool TcpClientMediator::SendData(const QByteArray& data, int len) {
    return m_net && m_net->SendData(data, len);
}

void TcpClientMediator::RecvData(const QByteArray& data, int len) {
    emit SIG_ReadyData(data, len);
}

}   // namespace net end