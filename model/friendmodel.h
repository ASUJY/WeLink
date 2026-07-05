#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "friend.hpp"
#include <QString>

class FriendModel
{
public:
    explicit FriendModel(const QString &connName = "userConn");
    ~FriendModel() = default;

    bool AddFriend(const uint64_t id, const Friend &fri);
    bool IsFriendExit(const uint64_t id, const Friend &fri);
    std::vector<Friend> FindFriends(const uint64_t id);

private:
    bool IsTableExit();

private:
    QString m_connName;
};

#endif // FRIENDMODEL_H
