#ifndef CONTACTSPANEWIDGET_H
#define CONTACTSPANEWIDGET_H

#include <QWidget>
#include <memory>
#include "user.hpp"
#include "contactsitem.h"
#include "friend.hpp"
#include "friendmodel.h"
#include "friendrequestmodel.h"

namespace Ui {
class ContactsPaneWidget;
}

class ContactsPaneWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_ItemDidSelected(std::shared_ptr<ContactsItem>);

public slots:
    void ReceiveSlotAddFriendReq(const QByteArray& data);
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReq(const User& user);
    void SlotAddFriendReqAck(const User& user);
    // void SlotItemDidSelected();

public:
    explicit ContactsPaneWidget(const std::shared_ptr<User>& user,
                                std::shared_ptr<FriendModel> friendModel,
                                std::shared_ptr<FriendRequestModel> friendRequestModel,
                                QWidget *parent = nullptr) noexcept;
    ~ContactsPaneWidget();
    void AddFriendToPane(const Friend& fri);
    void AddFriendToPane(const FriendRequest& fri);

private:
    std::unique_ptr<ContactsItem> CreateContactItem(const QString& groupName,
                                                    int64_t userId,
                                                    const QString& name,
                                                    FriendState state);

private:
    Ui::ContactsPaneWidget *ui;
    std::shared_ptr<User> m_user;
    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<FriendRequestModel> m_friendRequestModel;
};

#endif // CONTACTSPANEWIDGET_H
