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

    connect(ui->contactsListWidget, &ContactsListWidget::SIG_ItemDidSelected, this, &ContactsPaneWidget::SIG_ItemDidSelected);
}

ContactsPaneWidget::~ContactsPaneWidget()
{
    delete ui;
}

void ContactsPaneWidget::SlotAddFriendReq(User user) {
    ContactsItem *newFriendItem = new ContactsItem;
    newFriendItem->SetGroupName(tr("新的朋友"));
    newFriendItem->SetItemName(QString::fromStdString(user.GetUserName()));
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Send);
    ui->contactsListWidget->InsertItem(newFriendItem);
}

void ContactsPaneWidget::SlotReciveAddFriendReq(const QByteArray& data) {
    qDebug() << "ContactsPaneWidget::SlotReciveAddFriendReq";
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
    newFriendItem->SetItemId(friendid);
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Recevie);
    ui->contactsListWidget->InsertItem(newFriendItem);
}

void ContactsPaneWidget::SIG_ReciveAddFriendAckAgree(const QByteArray& data) {
    qDebug() << "ContactsPaneWidget::SIG_ReciveAddFriendAckAgree";
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
    newFriendItem->SetGroupName(tr("联系人"));
    newFriendItem->SetItemName(friendname);
    newFriendItem->SetItemId(friendid);
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Done);
    ui->contactsListWidget->InsertItem(newFriendItem);
}