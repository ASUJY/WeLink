#ifndef CONTACTSLISTVIEWCHILD_H
#define CONTACTSLISTVIEWCHILD_H

#include <QWidget>
#include <QMouseEvent>
#include "contactsitem.h"
#include "user.hpp"

class ContactsListViewChild : public QWidget
{
    Q_OBJECT

signals:
    void SIG_ItemDidSelected(ContactsListViewChild*);

public:
    explicit ContactsListViewChild(QWidget* parent = nullptr);
    ~ContactsListViewChild();

    void SetItem(ContactsItem* item) {m_item = item;}
    ContactsItem* GetItem() {return m_item;}

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    ContactsItem *m_item;
};

#endif // CONTACTSLISTVIEWCHILD_H
