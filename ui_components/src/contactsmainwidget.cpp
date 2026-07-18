#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"
#include "common.h"
#include <QButtonGroup>

ContactsMainWidget::ContactsMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3);

    QButtonGroup* btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);

    btnGroup->addButton(ui->btnAddFriend, 1);
    btnGroup->addButton(ui->btnRejectFriend, 2);

    connect(btnGroup, &QButtonGroup::buttonClicked, this, [=](QAbstractButton* btn){
        if (btn == ui->btnAddFriend) {
            SendSlotAddFriendACK(FriendState::ACCEPT);
            m_item->SetItemState(FriendState::ACCEPT);
        } else {
            SendSlotAddFriendACK(FriendState::REJECT);
            m_item->SetItemState(FriendState::REJECT);
        }
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
    if (item->GetItemState() == FriendState::PendingVerification) {
        ui->labName_3->setText(item->GetItemName());
        ui->btnHeadIcon_3->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_3->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(1);
    } else if (item->GetItemState() == FriendState::PendingApproval) {
        ui->labName->setText(item->GetItemName());
        ui->btnHeadIcon->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(0);
    } else if (item->GetItemState() == FriendState::DONE) {
        ui->labName_2->setText(item->GetItemName());
        ui->btnHeadIcon_2->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_2->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(2);
    } else if (item->GetItemState() == FriendState::ACCEPT) {
        ui->labName_4->setText(item->GetItemName());
        ui->btnHeadIcon_4->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_4->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(4);
    } else if (item->GetItemState() == FriendState::REJECT) {
        ui->labName_5->setText(item->GetItemName());
        ui->btnHeadIcon_5->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_5->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(5);
    } else {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void ContactsMainWidget::SendSlotAddFriendACK(FriendState type) {
    User user;
    user.SetUserName(m_item->GetItemName().toStdString());
    user.SetUserId(m_item->GetItemId());
    emit SIG_SEND_AddFriendReqAck(user, type);

}