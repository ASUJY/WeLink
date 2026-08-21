#ifndef FRIENDREQUEST_HPP
#define FRIENDREQUEST_HPP

#include "friend.hpp"
#include "common.h"

class FriendRequest : public Friend {
public:
    FriendRequest() = default;
    FriendRequest(const FriendRequest&) = delete;
    FriendRequest& operator=(const FriendRequest&) = delete;
    FriendRequest(FriendRequest&&) = default;
    FriendRequest& operator=(FriendRequest&&) = default;

    ~FriendRequest() = default;

    void SetFriendState(FriendState state) {
        m_state = state;
    }

    FriendState GetFriendState() const {
        return m_state;
    }

private:
    FriendState m_state;
};

#endif // FRIENDREQUEST_HPP
