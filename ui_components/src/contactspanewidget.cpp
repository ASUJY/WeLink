#include "contactspanewidget.h"
#include "ui_contactspanewidget.h"
#include "contactsitem.h"
#include "common.h"
#include <QJsonDocument>
#include <QJsonObject>

ContactsPaneWidget::ContactsPaneWidget(
    const std::shared_ptr<User>& user,
    std::shared_ptr<FriendModel> friendModel,
    std::shared_ptr<FriendRequestModel> friendRequestModel,
    QWidget *parent) noexcept
    : QWidget(parent),
    ui(new Ui::ContactsPaneWidget),
    m_user(user),
    m_friendModel(friendModel),
    m_friendRequestModel(friendRequestModel)
{
    ui->setupUi(this);

    // 初始化【新的朋友】分组
    std::unique_ptr<ContactsItem> groupNewFriend = std::make_unique<ContactsItem>();
    groupNewFriend->SetGroupName(tr("新的朋友"));
    groupNewFriend->SetItemType(ContactsItemType::Group);
    groupNewFriend->SetItemName(tr("新的朋友"));
    ui->contactsTreeView->InsertItem(std::move(groupNewFriend));

    // 初始化【联系人】分组
    std::unique_ptr<ContactsItem> groupContact = std::make_unique<ContactsItem>();
    groupContact->SetGroupName(tr("联系人"));
    groupContact->SetItemType(ContactsItemType::Group);
    groupContact->SetItemName(tr("联系人"));
    ui->contactsTreeView->InsertItem(std::move(groupContact));

    connect(ui->contactsTreeView, &ContactsTreeView::SIG_ItemDidSelected, this, &ContactsPaneWidget::SIG_ItemDidSelected);
}

ContactsPaneWidget::~ContactsPaneWidget()
{
    delete ui;
}

// 公共工厂函数：消除5处完全重复的ContactsItem构建逻辑
std::unique_ptr<ContactsItem> ContactsPaneWidget::CreateContactItem(const QString& groupName,
                                                                    int64_t userId,
                                                                    const QString& name,
                                                                    FriendState state)
{
    std::unique_ptr<ContactsItem> item = std::make_unique<ContactsItem>();
    item->SetGroupName(groupName);
    item->SetItemName(name);
    item->SetItemId(userId);
    item->SetHeadIcon(":/resource/head/man.svg");
    item->SetItemType(ContactsItemType::Item);
    item->SetItemState(state);
    return item;
}

void ContactsPaneWidget::SlotAddFriendReq(const User& user) {
    auto item = CreateContactItem(tr("新的朋友"),
                                  user.GetUserId(),
                                  QString::fromStdString(user.GetUserName()),
                                  FriendState::PendingVerification);
    ui->contactsTreeView->InsertItem(std::move(item));
    m_friendRequestModel->AddItem(m_user->GetUserId(), user, FriendState::PendingVerification);
}

void ContactsPaneWidget::ReceiveSlotAddFriendReq(const QByteArray& data) {
    qDebug() << "ContactsPaneWidget::SlotReciveAddFriendReq";
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError) || !jsonDoc.isObject()) {
        qWarning() << "解析好友请求JSON失败:" << jsonError.errorString();
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    QString friendname = dataObj.value("sendername").toString();
    int64_t friendid = static_cast<int64_t>(dataObj.value("senderid").toInteger());
    int64_t myid = static_cast<int64_t>(dataObj.value("receiverid").toInteger());

    User fri;
    fri.SetUserName(friendname.toStdString());
    fri.SetUserId(friendid);

    auto item = CreateContactItem(tr("新的朋友"), friendid, friendname, FriendState::PendingApproval);
    ui->contactsTreeView->InsertItem(std::move(item));

    m_friendRequestModel->AddItem(myid, fri, FriendState::PendingApproval);
}

void ContactsPaneWidget::SlotAddFriendReqAck(const User& user) {
    auto item = CreateContactItem(tr("联系人"),
                                  user.GetUserId(),
                                  QString::fromStdString(user.GetUserName()),
                                  FriendState::DONE);
    ui->contactsTreeView->InsertItem(std::move(item));
    m_friendModel->AddFriend(m_user->GetUserId(), user);
}

void ContactsPaneWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
    qDebug() << "ContactsPaneWidget::SIG_ReciveAddFriendAckAgree";
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError) || !jsonDoc.isObject()) {
        qWarning() << "解析同意好友ACK JSON失败:" << jsonError.errorString();
        return;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonValue dataVal = rootObj.value("data");

    QJsonObject dataObj = dataVal.toObject();
    QString friendname = dataObj.value("sendername").toString();
    int64_t friendid = static_cast<int64_t>(dataObj.value("senderid").toInteger());
    int64_t myid = static_cast<int64_t>(dataObj.value("receiverid").toInteger());
    // int acktype = dataObj.value("ackType").toString().toInt();

    // 更新【新的朋友】条目状态
    ui->contactsTreeView->UpdateItemState(tr("新的朋友"), friendid, FriendState::ACCEPT);
    m_friendRequestModel->UpdateItemStatus(myid, friendid,  FriendState::ACCEPT);

    // 在联系人分组新建条目
    auto item = CreateContactItem(tr("联系人"), friendid, friendname, FriendState::DONE);
    ui->contactsTreeView->InsertItem(std::move(item));

    Friend fri;
    fri.SetUserId(friendid);
    fri.SetUserName(friendname.toStdString());

    if (!m_friendModel->IsFriendExit(m_user->GetUserId(), fri)) {
        m_friendModel->AddFriend(m_user->GetUserId(), fri);
    }
}

void ContactsPaneWidget::AddFriendToPane(const Friend& fri) {
    auto item = CreateContactItem(tr("联系人"),
                                  fri.GetUserId(),
                                  QString::fromStdString(fri.GetUserName()),
                                  FriendState::DONE);
    ui->contactsTreeView->InsertItem(std::move(item));
}

void ContactsPaneWidget::AddFriendToPane(const FriendRequest& fri) {
    auto item = CreateContactItem(tr("新的朋友"),
                                  fri.GetUserId(),
                                  QString::fromStdString(fri.GetUserName()),
                                  fri.GetFriendState());
    ui->contactsTreeView->InsertItem(std::move(item));
}