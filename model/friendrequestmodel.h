#ifndef FRIENDREQUESTMODEL_H
#define FRIENDREQUESTMODEL_H

#include "user.hpp"
#include "common.h"
#include <QString>

class FriendRequestModel
{
public:
    explicit FriendRequestModel(const QString &connName = "userConn");
    ~FriendRequestModel() = default;

    bool AddItem(const int64_t id, const User &fri, FriendState status);
    bool UpdateItemStatus(const int64_t id, const int64_t friendid, FriendState status);
private:
    bool IsTableExit() const;

private:
    QString m_connName;

};

#endif // FRIENDREQUESTMODEL_H
