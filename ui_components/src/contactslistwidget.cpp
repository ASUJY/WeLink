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


void ContactsListWidget::InsertItem(std::unique_ptr<ContactsItem> item) {
    if (item->GetItemType() == ContactsItemType::Group) {
        m_items.push_back(std::move(item));
    } else {
        for (const std::shared_ptr<ContactsItem> &groupItem : m_items) {
            if (!groupItem->GetGroupName().compare(item->GetGroupName())) {
                // 往指定分组中添加联系人条目
                auto id = item->GetItemId();
                qDebug() << "ContactsListWidget::InsertItem: " << groupItem->GetGroupName() << " " << item->GetItemId() << " " << item->GetGroupName() << "" <<item->GetItemName() << " " << static_cast<int>(item->GetItemState());
                groupItem->AddChildItem(id, std::move(item));
                break;
            }
        }
    }
    // QTimer::singleShot(0, this, &ContactsListWidget::UploadItems);
    m_updateTimer->start(50);
}

void ContactsListWidget::UpdateItemStatus(const QString& groupname, int64_t id, FriendState state) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (!((*it)->GetGroupName().compare(groupname))) {
            auto item = (*it)->GetChildItemById(id);
            if (item) {
                item->SetItemState(state);
                break;
            }
        }
    }
    m_updateTimer->start(50);
}

void ContactsListWidget::UploadItems() {
    this->clear();
    for (auto w : findChildren<ContactsListViewGroup*>()) {
        w->deleteLater();
    }
    for (auto w : findChildren<ContactsListViewChild*>()) {
        w->deleteLater();
    }

    for (const auto& item : m_items) {
        if (item->GetItemType() == ContactsItemType::Group) {
            ContactsListViewGroup *group = new ContactsListViewGroup(item, this);   // 分组列表项样式
            group->setGeometry(0, 0, 300, 30);
            connect(group, &ContactsListViewGroup::SIG_GroupOpenStatusDidChanged, this, &ContactsListWidget::SlotGroupOpenDidChanged);

            QListWidgetItem *qitem = new QListWidgetItem("");
            this->addItem(qitem);
            this->setItemWidget(qitem, group);
            qitem->setSizeHint(group->geometry().size());
            if (item->GetIsOpen()) {
                for (auto &childItem : item->GetChildItems()) {
                    AddChildItem(childItem.second.get());
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
    ContactsListViewChild *childItem = new ContactsListViewChild(item->GetItemId(), item->GetItemName(), item->GetHeadIcon(),item->GetGroupName(), this);
    childItem->setGeometry(0, 0, 300, 60);
    // childItem->SetItem(item);
    connect(childItem, &ContactsListViewChild::SIG_ItemDidSelected, this, &ContactsListWidget::SlotItemDidSelected);

    QListWidgetItem *qitem = new QListWidgetItem("");
    this->addItem(qitem);
    this->setItemWidget(qitem, childItem);
    qitem->setSizeHint(childItem->geometry().size());
}

void  ContactsListWidget::SlotItemDidSelected(int64_t id, const QString& groupname) {
    for (auto it = m_items.cbegin(); it != m_items.end(); ++it) {
        if (!((*it)->GetGroupName().compare(groupname))) {
            auto item = (*it)->GetChildItemById(id);
            if (item) {
                emit SIG_ItemDidSelected(item);
                return;
            }
        }
    }
}