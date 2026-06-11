#include "contactspanewidget.h"
#include "ui_contactspanewidget.h"
#include "contactsitem.h"

ContactsPaneWidget::ContactsPaneWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsPaneWidget)
{
    ui->setupUi(this);

    ContactsItem *newFriendsItem = new ContactsItem;
    newFriendsItem->SetGroupName(tr("新的朋友"));
    newFriendsItem->SetIsOpen(false);
    newFriendsItem->SetItemType(ContactsItemType::Group);
    newFriendsItem->SetItemName(tr("新的朋友"));
    ui->contactsListWidget->InsertItem(newFriendsItem);

    ContactsItem *contactsItem = new ContactsItem;
    contactsItem->SetGroupName(tr("联系人"));
    contactsItem->SetIsOpen(false);
    contactsItem->SetItemType(ContactsItemType::Group);
    contactsItem->SetItemName(tr("联系人"));
    ui->contactsListWidget->InsertItem(contactsItem);
}

ContactsPaneWidget::~ContactsPaneWidget()
{
    delete ui;
}
