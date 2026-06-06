#ifndef COMMUNICATIONMEDIATOR_H
#define COMMUNICATIONMEDIATOR_H

#include <QObject>

namespace net {

class AbstractNet;

class CommunicationMediator : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationMediator(QObject *parent = nullptr){};
    virtual ~CommunicationMediator() = default;

    virtual bool SendData(const QByteArray& data, int len) = 0;

signals:

protected:
    AbstractNet* m_net = nullptr;
};

}   // namespace net end

#endif // COMMUNICATIONMEDIATOR_H
