#ifndef CONTACTSPANEWIDGET_H
#define CONTACTSPANEWIDGET_H

#include <QWidget>
#include "user.hpp"
#include "contactslistviewchild.h"

namespace Ui {
class ContactsPaneWidget;
}

class ContactsPaneWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_ItemDidSelected(ContactsListViewChild*);
public:
    explicit ContactsPaneWidget(QWidget *parent = nullptr);
    ~ContactsPaneWidget();

public slots:
    void SlotReciveAddFriendReq(const QByteArray& data);
    void SIG_ReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReq(User user);
    void SlotAddFriendReqAck(User user);
    // void SlotItemDidSelected();

private:
    Ui::ContactsPaneWidget *ui;
};

#endif // CONTACTSPANEWIDGET_H
