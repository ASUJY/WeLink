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
    explicit Friend(int wxid, QString name, QString image, bool mute, QString time, int count, QList<Message> messages):
        User(wxid, name.toStdString(), image.toStdString()), m_mute(mute), m_time(time), m_count(count), m_messages(messages){}


    void SetMute(bool mute) { m_mute = mute; }
    bool GetMute() const { return m_mute; }

    void SetTime(QString time) { m_time = time; }
    QString GetTime() const { return m_time; }

    void SetCount(int count) { m_count = count; }
    int GetCount() const { return m_count; }
    void AddCount(int num) { m_count += num; }

    // void SetMessages(QList<Message> messages) { m_messages = messages; }
    QList<Message> GetMessages() const { return  m_messages; }
    void AddMessage(Message& message) {m_messages.push_back(message);}
protected:

    bool m_mute;        // 表示好友是否被静音的状态
    QString m_time;     // 好友最后一条消息的时间
    int m_count;        // 未读消息数量
    QList<Message> m_messages;  // 好友的消息列表
};

#endif // FRIEND_HPP
