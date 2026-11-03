#include "contactswidget.h"
#include "ui_contactswidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QButtonGroup>


ContactsWidget::ContactsWidget(const std::shared_ptr<User>& user,
                               std::shared_ptr<FriendModel> friendModel,
                               std::shared_ptr<FriendRequestModel> friendRequestModel,
                               QWidget *parent) noexcept
    : QWidget(parent)
    , ui(new Ui::ContactsWidget),
    m_user(user),
    m_friendModel(friendModel),
    m_friendRequestModel(friendRequestModel)
{
    ui->setupUi(this);

    // contactspane init
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

    connect(ui->contactsTreeView, &ContactsTreeView::SIG_ItemDidSelected, this, &ContactsWidget::SIG_ItemDidSelected);


    // contactsmain init
    ui->stackedWidget->setCurrentIndex(3);

    QButtonGroup* btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);

    btnGroup->addButton(ui->btnAddFriend, 1);
    btnGroup->addButton(ui->btnRejectFriend, 2);

    connect(btnGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* btn){
        if (!m_item) {
            qWarning() << "ContactsWidget: m_item 为空，无法处理好友申请";
            return;
        }

        FriendState newState;
        if (btn == ui->btnAddFriend) {
            newState = FriendState::ACCEPT;
        } else {
            newState = FriendState::REJECT;
        }
        SendSlotAddFriendACK(newState);
        m_item->SetItemState(newState);

        SetStackedWidgetCurrentIndex(m_item);
    });
}

ContactsWidget::~ContactsWidget()
{
    delete ui;
}

// 公共工厂函数：消除5处完全重复的ContactsItem构建逻辑
auto ContactsWidget::CreateContactItem(const QString& groupName,
                                       int64_t userId,
                                       const QString& name,
                                       FriendState state) -> std::unique_ptr<ContactsItem>
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

void ContactsWidget::SlotAddFriendReq(const User& user) {
    auto item = CreateContactItem(tr("新的朋友"),
                                  user.GetUserId(),
                                  QString::fromStdString(user.GetUserName()),
                                  FriendState::PendingVerification);
    ui->contactsTreeView->InsertItem(std::move(item));
    m_friendRequestModel->AddItem(m_user->GetUserId(), user, FriendState::PendingVerification);
}

void ContactsWidget::ReceiveSlotAddFriendReq(const QByteArray& data) {
    qDebug() << "ContactsWidget::SlotReciveAddFriendReq";
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

void ContactsWidget::SlotAddFriendReqAck(const User& user) {
    auto item = CreateContactItem(tr("联系人"),
                                  user.GetUserId(),
                                  QString::fromStdString(user.GetUserName()),
                                  FriendState::DONE);
    ui->contactsTreeView->InsertItem(std::move(item));
    m_friendModel->AddFriend(m_user->GetUserId(), user);
}

void ContactsWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
    qDebug() << "ContactsWidget::SIG_ReciveAddFriendAckAgree";
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

void ContactsWidget::AddFriendToPane(const Friend& fri) {
    auto item = CreateContactItem(tr("联系人"),
                                  fri.GetUserId(),
                                  QString::fromStdString(fri.GetUserName()),
                                  FriendState::DONE);
    ui->contactsTreeView->InsertItem(std::move(item));
}

void ContactsWidget::AddFriendToPane(const FriendRequest& fri) {
    auto item = CreateContactItem(tr("新的朋友"),
                                  fri.GetUserId(),
                                  QString::fromStdString(fri.GetUserName()),
                                  fri.GetFriendState());
    ui->contactsTreeView->InsertItem(std::move(item));
}


void ContactsWidget::SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>& item) {
    m_item = item;
    if (!m_item) {
        ui->stackedWidget->setCurrentIndex(3);
        return;
    }

    FriendState state = m_item->GetItemState();
    const QString& name = m_item->GetItemName();
    QIcon headIcon(":/resource/head/man.svg");
    switch (state) {
    case FriendState::PendingVerification:
        ui->labName_3->setText(name);
        ui->btnHeadIcon_3->setIcon(headIcon);
        ui->labName_3->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case FriendState::PendingApproval:
        ui->labName->setText(name);
        ui->btnHeadIcon->setIcon(headIcon);
        ui->labName->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case FriendState::DONE:
        ui->labName_2->setText(name);
        ui->btnHeadIcon_2->setIcon(headIcon);
        ui->labName_2->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(2);
        break;
    case FriendState::ACCEPT:
        ui->labName_4->setText(name);
        ui->btnHeadIcon_4->setIcon(headIcon);
        ui->labName_4->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(4);
        break;
    case FriendState::REJECT:
        ui->labName_5->setText(name);
        ui->btnHeadIcon_5->setIcon(headIcon);
        ui->labName_5->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(5);
        break;
    default:
        ui->stackedWidget->setCurrentIndex(3);
        break;
    }
}

void ContactsWidget::SendSlotAddFriendACK(FriendState type) {
    if (!m_item) {
        qWarning() << "SendSlotAddFriendACK: m_item 为空";
        return;
    }

    User user;
    user.SetUserName(m_item->GetItemName().toStdString());
    user.SetUserId(m_item->GetItemId());
    emit SIG_SEND_AddFriendReqAck(user, type);

}