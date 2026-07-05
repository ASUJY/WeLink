#ifndef MSGMODEL_H
#define MSGMODEL_H

#include <QString>
#include "message.hpp"

class MsgModel
{
public:
    explicit MsgModel(const QString &connName = "userConn");
    ~MsgModel() = default;

    bool AddMsg(const uint64_t id, const uint64_t friId, const Message& msg);
    std::vector<Message> FindMsg(const uint64_t id, const uint64_t friId);

private:
    bool IsTableExit();

private:
    QString m_connName;
};

#endif // MSGMODEL_H
