#ifndef CONTACTSLISTWIDGET_H
#define CONTACTSLISTWIDGET_H

#include <QListWidget>
#include "contactsitem.h"

class ContactsListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ContactsListWidget(QWidget *parent = nullptr);

    void InsertItem(ContactsItem *item);

public slots:
    void SlotGroupOpenDidChanged();

private:
    void UploadItems();
    void AddChildItem(ContactsItem* item);

signals:

private:
    QList<ContactsItem*> m_items;
};

#endif // CONTACTSLISTWIDGET_H
