#ifndef CHATMSGLISTVIEW_H
#define CHATMSGLISTVIEW_H

#include <QListView>
#include "chatmsglistmodel.h"
#include "chatmsgdelegate.h"

class ChatMsgListView : public QListView
{
    Q_OBJECT
public:
    ChatMsgListView(QWidget *parent = nullptr);
    ~ChatMsgListView() = default;

    void SetMessage(const QList<ChatMessage>& item);
    void SetHeader(const QString &senderAvatar, const QString &receiverAvatar);

private:
    ChatMsgListModel* m_model = nullptr;
    ChatMsgDelegate* m_delegate = nullptr;
};

#endif // CHATMSGLISTVIEW_H
