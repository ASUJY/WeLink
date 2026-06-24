#ifndef CONTACTSMAINWIDGET_H
#define CONTACTSMAINWIDGET_H

#include <QWidget>
#include "contactslistviewchild.h"

namespace Ui {
class ContactsMainWidget;
}

class ContactsMainWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_AddFriendReqAck(const User& user);
public:
    explicit ContactsMainWidget(QWidget *parent = nullptr);
    ~ContactsMainWidget();
    void SetStackedWidgetCurrentIndex(ContactsListViewChild*);

public slots:
    void SlotAddFriendAgree();

private:
    Ui::ContactsMainWidget *ui;
    ContactsListViewChild *m_item = nullptr;
};

#endif // CONTACTSMAINWIDGET_H
