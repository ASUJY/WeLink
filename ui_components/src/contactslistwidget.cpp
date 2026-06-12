#include "contactslistwidget.h"
#include "contactslistviewgroup.h"
#include "contactslistviewchild.h"

ContactsListWidget::ContactsListWidget(QWidget *parent)
    : QListWidget{parent}
{}


void ContactsListWidget::InsertItem(ContactsItem* item) {
    if (item->GetItemType() == ContactsItemType::Group) {
        m_items.append(item);
    } else {
        foreach (ContactsItem *groupItem, m_items) {
            if (!groupItem->GetGroupName().compare(item->GetGroupName())) {
                // 往指定分组中添加联系人条目
                groupItem->AddChildItem(item);
                break;
            }
        }
    }
    UploadItems();
}

void ContactsListWidget::UploadItems() {
    this->clear();
    for (ContactsItem* item : m_items) {
        if (item->GetItemType() == ContactsItemType::Group) {
            ContactsListViewGroup *group = new ContactsListViewGroup;
            group->setGeometry(0, 0, 300, 30);
            group->SetLabTitle(item);
            group->SetGroupState(item);
            connect(group, &ContactsListViewGroup::SIG_GroupOpenStatusDidChanged, this, &ContactsListWidget::SlotGroupOpenDidChanged);

            QListWidgetItem *qitem = new QListWidgetItem("");
            this->addItem(qitem);
            this->setItemWidget(qitem, group);
            qitem->setSizeHint(group->geometry().size());
            if (item->GetIsOpen()) {
                for (ContactsItem *childItem : item->GetChildItems()) {
                    AddChildItem(childItem);
                }
            }
        }
    }
}

void ContactsListWidget::SlotGroupOpenDidChanged()
{
    UploadItems();
}

void ContactsListWidget::AddChildItem(ContactsItem* item) {
    ContactsListViewChild *childItem = new ContactsListViewChild;
    childItem->setGeometry(0, 0, 300, 60);
    childItem->SetItem(item);

    QListWidgetItem *qitem = new QListWidgetItem("");
    this->addItem(qitem);
    this->setItemWidget(qitem, childItem);
    qitem->setSizeHint(childItem->geometry().size());

}