#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "chatlistitem.h"

enum PageType {
    AllChatView,
};

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

signals:
    void SIG_SendChatMsg(int64_t id, const QString& content);
    void SIG_AddFriend();
    void SIG_ItemClicked(QVariant var, PageType type);

public slots:
    void SlotReciveAddFriendAckAgree(const QByteArray& data);   // 接收到对方发来的同意添加朋友信息，则在聊天列表中新增这个朋友的聊天项
    void SlotAddFriendReqAck(const User& user);                 // 同意添加对方为好友，则在聊天列表中新增这个朋友的聊天项

private slots:
    void SlotBtnSendMsgClicked();
    void SlotItemSelected(ChatListItem *item);

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void SetData(Friend *data);
    void SetStackedWidgetCurrentIndex(int index);
    static ChatListItem* GetCurrentSelectedItem() noexcept { return m_selectedItem; }
    ChatListItem* GetItemById(int64_t id) const;
    void AddFriendToPane(std::unique_ptr<Friend>&& fri);

private:
    // 抽取创建聊天项公共函数(UI创建代码)
    void CreateChatListItem(int64_t friendId, std::unique_ptr<Friend>&& fri);

private:
    Ui::ChatWidget *ui;

    // 好友id与好友列表项的映射关系
    std::map<int64_t, ChatListItem*> m_mapIdToChatItem;

    static ChatListItem *m_selectedItem;    // 当前被选中的聊天项
};

#endif // CHATWIDGET_H
