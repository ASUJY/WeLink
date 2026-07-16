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
    void SIG_SEND_AddFriendReqAck(const User& user, E_ACK_TYPE type);
public:
    explicit ContactsMainWidget(QWidget *parent = nullptr);
    ~ContactsMainWidget();
    void SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>&);

public slots:
    void SendSlotAddFriendACK(E_ACK_TYPE type);

private:
    Ui::ContactsMainWidget *ui;
    std::shared_ptr<ContactsItem> m_item = nullptr;
};

#endif // CONTACTSMAINWIDGET_H
