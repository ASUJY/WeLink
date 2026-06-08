#ifndef ABSTRACTNET_H
#define ABSTRACTNET_H

#include <QObject>
#include "communicationmediator.h"

namespace net {

class AbstractNet : public QObject
{
    Q_OBJECT
public:
    explicit AbstractNet(QObject *parent = nullptr) {};
    virtual ~AbstractNet() = default;

    virtual bool OpenNetwork() = 0;
    virtual void CloseNetwork() = 0;
    virtual bool SendData(const QByteArray& data, int len) = 0;

public slots:
    virtual void RecvData() = 0;

signals:

protected:
    CommunicationMediator* m_commMediator = nullptr;
    // bool m_isOpenNetwork = false;
};

}   // namespace net end

#endif // ABSTRACTNET_H
