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

    bool AddFriend(const uint64_t id, const User &fri, FrinedState state);
    bool IsFriendExit(const uint64_t id, const Friend &fri);
    User IsFriendExit(const uint64_t id, const QString &name, E_ACCOUNT_TYPE type);
    std::vector<Friend> FindFriends(const uint64_t id);
    bool UpdateFriendState(const uint64_t id, const uint64_t friendid, E_ACK_TYPE type);

private:
    bool IsTableExit();

private:
    QString m_connName;
};

#endif // FRIENDMODEL_H
