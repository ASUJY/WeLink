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

    std::unique_ptr<ContactsItem> newFriendsItem = std::make_unique<ContactsItem>();
    newFriendsItem->SetGroupName(tr("新的朋友"));
    newFriendsItem->SetIsOpen(false);
    newFriendsItem->SetItemType(ContactsItemType::Group);
    newFriendsItem->SetItemName(tr("新的朋友"));
    ui->contactsListWidget->InsertItem(std::move(newFriendsItem));

    std::unique_ptr<ContactsItem> contactsItem = std::make_unique<ContactsItem>();
    contactsItem->SetGroupName(tr("联系人"));
    contactsItem->SetIsOpen(false);
    contactsItem->SetItemType(ContactsItemType::Group);
    contactsItem->SetItemName(tr("联系人"));
    ui->contactsListWidget->InsertItem(std::move(contactsItem));

    connect(ui->contactsListWidget, &ContactsListWidget::SIG_ItemDidSelected, this, &ContactsPaneWidget::SIG_ItemDidSelected);
}

ContactsPaneWidget::~ContactsPaneWidget()
{
    delete ui;
}

void ContactsPaneWidget::SlotAddFriendReq(const User& user) {
    qDebug() << "ContactsPaneWidget::SlotAddFriendReq start";
    std::unique_ptr<ContactsItem> newFriendItem = std::make_unique<ContactsItem>();
    newFriendItem->SetGroupName(tr("新的朋友"));
    newFriendItem->SetItemName(QString::fromStdString(user.GetUserName()));
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Send);
    ui->contactsListWidget->InsertItem(std::move(newFriendItem));
    qDebug() << "ContactsPaneWidget::SlotAddFriendReq end";
}

void ContactsPaneWidget::SlotAddFriendReqAck(const User& user) {
    std::unique_ptr<ContactsItem> newFriendItem = std::make_unique<ContactsItem>();
    newFriendItem->SetGroupName(tr("联系人"));
    newFriendItem->SetItemName(QString::fromStdString(user.GetUserName()));
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Done);
    ui->contactsListWidget->InsertItem(std::move(newFriendItem));
}

void ContactsPaneWidget::ReceiveSlotAddFriendReq(const QByteArray& data) {
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

    std::unique_ptr<ContactsItem> newFriendItem = std::make_unique<ContactsItem>();
    newFriendItem->SetGroupName(tr("新的朋友"));
    newFriendItem->SetItemName(friendname);
    newFriendItem->SetItemId(friendid);
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Recevie);
    ui->contactsListWidget->InsertItem(std::move(newFriendItem));
}

void ContactsPaneWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
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

    std::unique_ptr<ContactsItem> newFriendItem = std::make_unique<ContactsItem>();
    newFriendItem->SetGroupName(tr("联系人"));
    newFriendItem->SetItemName(friendname);
    newFriendItem->SetItemId(friendid);
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Done);
    ui->contactsListWidget->InsertItem(std::move(newFriendItem));
}

void ContactsPaneWidget::AddFriendToPane(const Friend& fri) {
    std::unique_ptr<ContactsItem> newFriendItem = std::make_unique<ContactsItem>();
    newFriendItem->SetGroupName(tr("联系人"));
    newFriendItem->SetItemName(QString::fromStdString(fri.GetUserName()));
    newFriendItem->SetItemId(fri.GetUserId());
    newFriendItem->SetHeadIcon(":/resource/head/man.svg");
    newFriendItem->SetItemType(ContactsItemType::Item);
    newFriendItem->SetItemState(ContactsState::Done);
    ui->contactsListWidget->InsertItem(std::move(newFriendItem));
}