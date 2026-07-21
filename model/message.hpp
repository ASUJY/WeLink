#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>

enum MsgType {
    None = 0,
    Sender = 1,
    Receive = 2,
};

class Message
{
public:
    explicit Message() = default;
    explicit Message(const QString& content, const QString& time, MsgType msgType):
        m_content(content), m_time(time), m_msgType(msgType){}
    Message(Message&& other) noexcept : m_content(std::move(other.m_content)),
        m_time(std::move(other.m_time)),
        m_msgType(other.m_msgType) {}
    Message& operator=(Message&& other) noexcept {
        if (this != &other) {
            m_content = std::move(other.m_content);
            m_time = std::move(other.m_time);
            m_msgType = other.m_msgType;
        }
        return *this;
    }
    Message(const Message&) = default;
    Message& operator=(const Message&) = default;
    ~Message() = default;

    void SetContent(const QString& content) noexcept { m_content = content; }
    void SetTime(const QString& time) noexcept { m_time = time; }
    void SetMsgType(MsgType msgType) noexcept { m_msgType = msgType; }

    const QString& GetContent() const { return m_content; }
    const QString& GetTime() const { return m_time; }
    MsgType GetMsgType() const { return m_msgType; }

private:
    QString m_content{};   // 消息内容
    QString m_time{};      // 消息的发送时间
    MsgType m_msgType = MsgType::None;   // 消息来源类别
};

#endif // MESSAGE_H
