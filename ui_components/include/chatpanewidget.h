#ifndef CHATPANEWIDGET_H
#define CHATPANEWIDGET_H

#include <QWidget>
#include "chatlistitem.h"

enum PageType {
    AllChatView,
};


namespace Ui {
class ChatPaneWidget;
}

class ChatPaneWidget : public QWidget
{
    Q_OBJECT

signals:
    void SIG_AddFriend();
    void SIG_ItemClicked(QVariant var, PageType type);

public:
    explicit ChatPaneWidget(QWidget *parent = nullptr);
    ~ChatPaneWidget();
    static ChatListItem* GetItem() { return m_item; }
    ChatListItem* GetItemById(uint64_t id);

public slots:
    // 接收到对方发来的同意添加朋友信息，则在聊天列表中新增这个朋友的聊天项
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    // 同意添加对方为好友，则在聊天列表中新增这个朋友的聊天项
    void SlotAddFriendReqAck(const User& user);

private slots:
    void SlotItemSelected(ChatListItem *item);

private:
    Ui::ChatPaneWidget *ui;
    // 好友id与好友列表项的映射关系
    std::map<uint64_t, ChatListItem*> m_mapIdToChatItem;

    static ChatListItem *m_item;    // 当前被选中的聊天项
};

#endif // CHATPANEWIDGET_H
