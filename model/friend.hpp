#ifndef FRIEND_HPP
#define FRIEND_HPP

#include <QObject>
#include <QString>
#include "user.hpp"
#include "message.hpp"


class Friend : public User
{
public:
    explicit Friend() = default;
    explicit Friend(int64_t wxid,
                    const QString& name,
                    const QString& image,
                    bool mute,
                    const QString& time, int count, const QList<Message>& messages):
        User(wxid, name.toStdString(), image.toStdString()),
        m_mute(mute),
        m_time(time),
        m_count(count),
        m_messages(messages),
        m_state(FriendState::REJECT){}


    void SetMute(bool mute) { m_mute = mute; }
    void SetTime(const QString& time) { m_time = time; }
    void SetCount(int count) { m_count = count; }
    void AddMessage(const Message& message)  {m_messages.push_back(message);}
    void AddMessage(Message&& message) noexcept { m_messages.push_back(std::move(message));}
    template<typename... Args>
    void EmplaceMessage(Args&&... args)
    {
        m_messages.emplace_back(std::forward<Args>(args)...);
    }
    void SetState(FriendState state) { m_state = state; }
    void SetMessages(const QList<Message>& messages) { m_messages = messages; }


    bool GetMute() const { return m_mute; }
    const QString& GetTime() const { return m_time; }
    int GetCount() const { return m_count; }
    void AddCount(int num) { m_count += num; }
    const QList<Message>& GetMessages() const { return m_messages; }
    FriendState GetState() const { return m_state; }

    Friend(const Friend&) = delete;
    Friend& operator=(const Friend&) = delete;
    Friend(Friend&&) = default;
    Friend& operator=(Friend&&) = default;

protected:
    bool m_mute = false;                // 表示好友是否被静音的状态

    QString m_time;             // 好友最后一条消息的时间
    int m_count = 0;                // 未读消息数量
    QList<Message> m_messages;  // 好友的消息列表

    FriendState m_state = FriendState::REJECT;        // 好友状态，是否已经互相添加为好友
};

#endif // FRIEND_HPP
