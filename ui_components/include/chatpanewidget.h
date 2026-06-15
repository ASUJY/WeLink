#ifndef CHATPANEWIDGET_H
#define CHATPANEWIDGET_H

#include <QWidget>
#include "chatlistitem.h"

namespace Ui {
class ChatPaneWidget;
}

class ChatPaneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPaneWidget(QWidget *parent = nullptr);
    ~ChatPaneWidget();

public slots:
    void SlotReciveAddFriendReq(const QByteArray& data);

signals:
    void SIG_AddFriend();

private:
    Ui::ChatPaneWidget *ui;
    // 好友id与好友列表项的映射关系
    std::map<int, ChatListItem*> m_mapIdToChatItem;
};

#endif // CHATPANEWIDGET_H
