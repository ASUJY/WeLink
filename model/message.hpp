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
        _content(content), _time(time), _msgType(msgType){}

    void SetContent(QString content) { _content = content; }
    QString GetContent() const { return _content; }

    void SetTime(QString time) { _time = time; }
    QString GetTime() const { return _time; }

    void SetMsgType(MsgType msgType) { _msgType = msgType; }
    MsgType GetMsgType() const { return _msgType; }

private:

    QString _content;   // 消息内容
    QString _time;      // 消息的发送时间
    MsgType _msgType;   // 消息来源类别
};

#endif // MESSAGE_H
