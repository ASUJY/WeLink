#include "tcpclientmediator.h"
#include "tcpclient.h"

namespace net {
TcpClientMediator::TcpClientMediator() {
    m_net = new TcpClient(this);
}

TcpClientMediator::~TcpClientMediator() {
    if (m_net) {
        delete m_net;
        m_net = nullptr;
    }
}

bool TcpClientMediator::SendData(const QByteArray& data, int len) {
    if (!m_net || !m_net->SendData(data, len)) {
        return false;
    }

    return true;
}

void TcpClientMediator::RecvData(const QByteArray& data, int len) {
    emit SIG_ReadyData(data, len);
}

}   // namespace net end