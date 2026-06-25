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
    explicit ContactsListViewGroup(const std::shared_ptr<ContactsItem>& item, QWidget *parent = nullptr);
    ~ContactsListViewGroup();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void SetGroupState();
    void SetLabTitle();

private:
    Ui::ContactsListViewGroup *ui;
    std::shared_ptr<ContactsItem> m_item;

};

#endif // CONTACTSLISTVIEWGROUP_H
