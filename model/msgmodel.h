#ifndef MSGMODEL_H
#define MSGMODEL_H

#include <QString>
#include "message.hpp"

class MsgModel
{
public:
    explicit MsgModel(const QString &connName = "userConn");
    ~MsgModel() = default;

    bool AddMsg(const int64_t id, const int64_t friId, const Message& msg);
    std::vector<Message> FindMsg(const int64_t id, const int64_t friId) const;

private:
    bool IsTableExit() const;

private:
    QString m_connName;
};

#endif // MSGMODEL_H
