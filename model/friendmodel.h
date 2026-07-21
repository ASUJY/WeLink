#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "friend.hpp"
#include "common.h"
#include <QString>

class FriendModel
{
public:
    explicit FriendModel(const QString &connName = "userConn");
    ~FriendModel() = default;

    bool AddFriend(const int64_t id, const User &fri);
    bool IsFriendExit(const int64_t id, const Friend &fri) const;
    User IsFriendExit(const int64_t id, const QString &name, E_ACCOUNT_TYPE type) const;
    std::vector<Friend> FindFriends(const int64_t id) const;
    // bool UpdateFriendState(const int64_t id, const int64_t friendid, FriendState state);

private:
    bool IsTableExit() const;

private:
    QString m_connName;
};

#endif // FRIENDMODEL_H
