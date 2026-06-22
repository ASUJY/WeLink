#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"

ContactsMainWidget::ContactsMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3);
    connect(ui->btnAddFriend, &QPushButton::clicked, this, &ContactsMainWidget::SlotAddFriendAgree);
}

ContactsMainWidget::~ContactsMainWidget()
{
    delete ui;
}

void ContactsMainWidget::SetStackedWidgetCurrentIndex(ContactsListViewChild *item) {
    m_item = item;
    if (item->GetItem()->GetItemState() == ContactsState::Send) {
        ui->labName_3->setText(item->GetItem()->GetItemName());
        ui->btnHeadIcon_3->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_3->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(1);
    } else if (item->GetItem()->GetItemState() == ContactsState::Recevie) {
        ui->labName->setText(item->GetItem()->GetItemName());
        ui->btnHeadIcon->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(0);
    } else if (item->GetItem()->GetItemState() == ContactsState::Done) {
        ui->labName_2->setText(item->GetItem()->GetItemName());
        ui->btnHeadIcon_2->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_2->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(2);
    } else {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void ContactsMainWidget::SlotAddFriendAgree() {
    qDebug() << "ContactsMainWidget::SlotAddFriendAgree";
    User user;
    user.SetUserName(m_item->GetItem()->GetItemName().toStdString());
    user.SetUserId(m_item->GetItem()->GetItemId());
    emit SIG_AddFriendReqAck(user);
}