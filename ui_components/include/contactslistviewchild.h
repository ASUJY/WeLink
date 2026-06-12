#ifndef CONTACTSLISTVIEWCHILD_H
#define CONTACTSLISTVIEWCHILD_H

#include <QWidget>
#include "contactsitem.h"

class ContactsListViewChild : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsListViewChild(QWidget* parent = nullptr);
    ~ContactsListViewChild();

    void SetItem(ContactsItem* item) {m_item = item;}
    ContactsItem* GetItem() {return m_item;}

protected:
    void paintEvent(QPaintEvent *);

private:
    ContactsItem *m_item;
};

#endif // CONTACTSLISTVIEWCHILD_H
