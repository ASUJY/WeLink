#ifndef CONTACTSTREEVIEW_H
#define CONTACTSTREEVIEW_H

#include <QTreeView>
#include "contactsitem.h"
#include "contactstreemodel.h"
#include "contactsitemdelegate.h"

class ContactsTreeView : public QTreeView
{
    Q_OBJECT

signals:
    void SIG_ItemDidSelected(const std::shared_ptr<ContactsItem>& item);

private slots:
    void OnClicked(const QModelIndex& index);

public:
    explicit ContactsTreeView(QWidget *parent = nullptr);
    ~ContactsTreeView() = default;

    void InsertItem(std::unique_ptr<ContactsItem> item);
    void UpdateItemState(const QString& groupname, int64_t id, FriendState state);

private:
    ContactsTreeModel* m_model = nullptr;
    ContactsItemDelegate* m_delegate = nullptr;
};

#endif // CONTACTSTREEVIEW_H
