#include "contactslistwidget.h"
#include "contactslistviewgroup.h"
#include "contactslistviewchild.h"
#include <QTimer>

ContactsListWidget::ContactsListWidget(QWidget *parent)noexcept
    : QListWidget{parent}
{
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    connect(m_updateTimer, &QTimer::timeout, this, &ContactsListWidget::UploadItems);
}


void ContactsListWidget::InsertItem(std::unique_ptr<ContactsItem>&& item) {
    if (!item)
        return;

    if (item->GetItemType() == ContactsItemType::Group) {
        m_items.push_back(std::move(item));
    } else {
        const QString targetGroup = item->GetGroupName();
        for (const std::shared_ptr<ContactsItem> &groupItem : m_items) {
            if (groupItem->GetGroupName()== targetGroup) {
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
        if ((*it)->GetGroupName() == groupname) {
            auto childItem  = (*it)->GetChildItemById(id);
            if (childItem) {
                childItem ->SetItemState(state);
                break;
            }
        }
    }
    m_updateTimer->start(50);
}

void ContactsListWidget::UploadItems() {
    this->clear();
    // 安全销毁所有自定义控件
    for (auto w : findChildren<ContactsListViewGroup*>()) {
        w->deleteLater();
    }
    for (auto w : findChildren<ContactsListViewChild*>()) {
        w->deleteLater();
    }

    for (const auto& groupItem : m_items) {
        if (groupItem->GetItemType() != ContactsItemType::Group) {
            continue;
        }

        ContactsListViewGroup *groupWidget  = new ContactsListViewGroup(groupItem , this);   // 分组列表项样式
        groupWidget->setGeometry(0, 0, 300, 30);
        connect(groupWidget, &ContactsListViewGroup::SIG_GroupOpenStatusDidChanged, this, &ContactsListWidget::SlotGroupOpenDidChanged);

        QListWidgetItem *qlistItem  = new QListWidgetItem("");
        this->addItem(qlistItem );
        this->setItemWidget(qlistItem , groupWidget);
        qlistItem ->setSizeHint(groupWidget->geometry().size());

        if (groupItem ->GetIsOpen()) {
            const auto& childMap = groupItem->GetChildItems();
            for (const auto &childItem : childMap) {
                AddChildItem(childItem.second.get());
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
    if (!item)
        return;

    ContactsListViewChild *childWidget = new ContactsListViewChild(
        item->GetItemId(),
        item->GetItemName(),
        item->GetHeadIcon(),
        item->GetGroupName(),
        this
    );
    childWidget->setGeometry(0, 0, 300, 60);
    // childItem->SetItem(item);
    connect(childWidget, &ContactsListViewChild::SIG_ItemDidSelected, this, &ContactsListWidget::SlotItemDidSelected);

    QListWidgetItem *qlistItem  = new QListWidgetItem("");
    this->addItem(qlistItem );
    this->setItemWidget(qlistItem , childWidget);
    qlistItem->setSizeHint(childWidget->geometry().size());
}

void  ContactsListWidget::SlotItemDidSelected(int64_t id, const QString& groupname) {
    for (auto it = m_items.cbegin(); it != m_items.end(); ++it) {
        if (((*it)->GetGroupName() == groupname)) {
            auto childItem  = (*it)->GetChildItemById(id);
            if (childItem ) {
                emit SIG_ItemDidSelected(childItem );
                return;
            }
        }
    }
}