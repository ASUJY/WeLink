#ifndef CONTACTSLISTVIEWGROUP_H
#define CONTACTSLISTVIEWGROUP_H

#include <QWidget>
#include <QMouseEvent>
#include "contactsitem.h"

namespace Ui {
class ContactsListViewGroup;
}

class ContactsListViewGroup : public QWidget
{
    Q_OBJECT

signals:
    void SIG_GroupOpenStatusDidChanged();

public:
    explicit ContactsListViewGroup(QWidget *parent = nullptr);
    ~ContactsListViewGroup();

    void SetGroupState(ContactsItem *item);
    void SetLabTitle(ContactsItem *item);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::ContactsListViewGroup *ui;
    ContactsItem *m_item = nullptr;
};

#endif // CONTACTSLISTVIEWGROUP_H
