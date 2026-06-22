#include "contactslistwidget.h"
#include "contactslistviewgroup.h"
#include "contactslistviewchild.h"
#include <QTimer>

ContactsListWidget::ContactsListWidget(QWidget *parent)
    : QListWidget{parent}
{
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    connect(m_updateTimer, &QTimer::timeout, this, &ContactsListWidget::UploadItems);
}


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
    // QTimer::singleShot(0, this, &ContactsListWidget::UploadItems);
    m_updateTimer->start(50);
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
    // QTimer::singleShot(0, this, &ContactsListWidget::UploadItems);
    m_updateTimer->start(50);
}

void ContactsListWidget::AddChildItem(ContactsItem* item) {
    ContactsListViewChild *childItem = new ContactsListViewChild;
    childItem->setGeometry(0, 0, 300, 60);
    childItem->SetItem(item);
    connect(childItem, &ContactsListViewChild::SIG_ItemDidSelected, this, &ContactsListWidget::SIG_ItemDidSelected);

    QListWidgetItem *qitem = new QListWidgetItem("");
    this->addItem(qitem);
    this->setItemWidget(qitem, childItem);
    qitem->setSizeHint(childItem->geometry().size());

}