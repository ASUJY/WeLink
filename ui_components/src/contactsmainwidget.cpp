#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"
#include "common.h"
#include <QButtonGroup>

ContactsMainWidget::ContactsMainWidget(QWidget *parent) noexcept
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3);

    QButtonGroup* btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);

    btnGroup->addButton(ui->btnAddFriend, 1);
    btnGroup->addButton(ui->btnRejectFriend, 2);

    connect(btnGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* btn){
        if (!m_item) {
            qWarning() << "ContactsMainWidget: m_item 为空，无法处理好友申请";
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

    // connect(ui->btnAddFriend, &QPushButton::clicked, this, &ContactsMainWidget::SlotAddFriendAgree);
}

ContactsMainWidget::~ContactsMainWidget()
{
    delete ui;
}

void ContactsMainWidget::SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>& item) {
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

void ContactsMainWidget::SendSlotAddFriendACK(FriendState type) {
    if (!m_item) {
        qWarning() << "SendSlotAddFriendACK: m_item 为空";
        return;
    }

    User user;
    user.SetUserName(m_item->GetItemName().toStdString());
    user.SetUserId(m_item->GetItemId());
    emit SIG_SEND_AddFriendReqAck(user, type);

}