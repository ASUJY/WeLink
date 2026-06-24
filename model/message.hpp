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
    explicit Message(QString content, QString time, MsgType msgType):
        m_content(content), m_time(time), m_msgType(msgType){}
    ~Message() = default;

    void SetContent(const QString content) { m_content = content; }
    void SetTime(const QString time) { m_time = time; }
    void SetMsgType(MsgType msgType) { m_msgType = msgType; }

    QString GetContent() const { return m_content; }
    QString GetTime() const { return m_time; }
    MsgType GetMsgType() const { return m_msgType; }

private:
    QString m_content;   // 消息内容
    QString m_time;      // 消息的发送时间
    MsgType m_msgType;   // 消息来源类别
};

#endif // MESSAGE_H
