#ifndef CONTACTSMAINWIDGET_H
#define CONTACTSMAINWIDGET_H

#include <QWidget>
#include "contactsitem.h"
#include "user.hpp"
#include "common.h"

namespace Ui {
class ContactsMainWidget;
}

class ContactsMainWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_SEND_AddFriendReqAck(const User& user, FriendState type);

public slots:
    void SendSlotAddFriendACK(FriendState type);

public:
    explicit ContactsMainWidget(QWidget *parent = nullptr) noexcept;
    ~ContactsMainWidget();
    void SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>&);

private:
    Ui::ContactsMainWidget *ui;
    std::shared_ptr<ContactsItem> m_item;
};

#endif // CONTACTSMAINWIDGET_H
