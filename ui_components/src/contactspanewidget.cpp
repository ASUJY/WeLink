#include "contactspanewidget.h"
#include "ui_contactspanewidget.h"
#include "contactsitem.h"

#include <QJsonDocument>
#include <QJsonObject>

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


void ContactsPaneWidget::SlotReciveAddFriendReq(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    // if (jsontmp.isNull() || !jsontmp.isObject()) return;

    QJsonObject dataObj = dataVal.toObject();
    QString friendname = dataObj.value("sendername").toString();
    int friendid = dataObj.value("senderid").toInt();

    ContactsItem *newFriendItem = new ContactsItem;
    newFriendItem->SetGroupName(tr("新的朋友"));
    newFriendItem->SetItemName(friendname);
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    ui->contactsListWidget->InsertItem(newFriendItem);
}