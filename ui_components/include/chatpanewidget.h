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

public:
    explicit ChatPaneWidget(QWidget *parent = nullptr);
    ~ChatPaneWidget();
    static ChatListItem* GetItem() { return m_item; }
    ChatListItem* GetItemById(int id);

public slots:
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReqAck(User user);

private slots:
    void SlotItemSelected(ChatListItem *item);

signals:
    void SIG_AddFriend();
    void SIG_ItemClicked(QVariant var, PageType type);

private:
    Ui::ChatPaneWidget *ui;
    // 好友id与好友列表项的映射关系
    std::map<int, ChatListItem*> m_mapIdToChatItem;

    static ChatListItem *m_item;    // 当前被选中的聊天项
};

#endif // CHATPANEWIDGET_H
