#include "contactslistwidget.h"
#include "contactslistviewgroup.h"

ContactsListWidget::ContactsListWidget(QWidget *parent)
    : QListWidget{parent}
{}


void ContactsListWidget::InsertItem(ContactsItem* item) {
    if (item->GetItemType() == ContactsItemType::Group) {
        m_items.append(item);
    }
    UploadItems();
}

void ContactsListWidget::UploadItems() {
    this->clear();

    for (ContactsItem* item : m_items) {
        if (item->GetItemType() == ContactsItemType::Group) {
            ContactsListViewGroup *group = new ContactsListViewGroup;
            // group->setGeometry(0, 0, 250, 635);
            group->SetLabTitle(item);
            group->SetGroupState(item);

            QListWidgetItem *qitem = new QListWidgetItem("");
            this->addItem(qitem);
            this->setItemWidget(qitem, group);
            qitem->setSizeHint(group->geometry().size());
        }
    }
}