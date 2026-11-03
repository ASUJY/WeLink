#ifndef CONTACTSWIDGET_H
#define CONTACTSWIDGET_H

#include <QWidget>
#include "contactsitem.h"
#include "user.hpp"
#include "friend.hpp"
#include "friendmodel.h"
#include "friendrequestmodel.h"

namespace Ui {
class ContactsWidget;
}

class ContactsWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_ItemDidSelected(std::shared_ptr<ContactsItem>);
    void SIG_SEND_AddFriendReqAck(const User& user, FriendState type);

public slots:
    void ReceiveSlotAddFriendReq(const QByteArray& data);
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReq(const User& user);
    void SlotAddFriendReqAck(const User& user);
    void SendSlotAddFriendACK(FriendState type);

public:
    explicit ContactsWidget(const std::shared_ptr<User>& user,
                            std::shared_ptr<FriendModel> friendModel,
                            std::shared_ptr<FriendRequestModel> friendRequestModel,
                            QWidget *parent = nullptr) noexcept;
    ~ContactsWidget();

    void AddFriendToPane(const Friend& fri);
    void AddFriendToPane(const FriendRequest& fri);
    void SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>&);

private:
    std::unique_ptr<ContactsItem> CreateContactItem(const QString& groupName,
                                                    int64_t userId,
                                                    const QString& name,
                                                    FriendState state);

private:
    Ui::ContactsWidget *ui;

    std::shared_ptr<User> m_user;
    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<FriendRequestModel> m_friendRequestModel;

    std::shared_ptr<ContactsItem> m_item;
};

#endif // CONTACTSWIDGET_H
