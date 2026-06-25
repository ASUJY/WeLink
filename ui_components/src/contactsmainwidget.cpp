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

void ContactsMainWidget::SetStackedWidgetCurrentIndex(const std::shared_ptr<ContactsItem>& item) {
    m_item = item;
    if (item->GetItemState() == ContactsState::Send) {
        ui->labName_3->setText(item->GetItemName());
        ui->btnHeadIcon_3->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName_3->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(1);
    } else if (item->GetItemState() == ContactsState::Recevie) {
        ui->labName->setText(item->GetItemName());
        ui->btnHeadIcon->setIcon(QIcon(":/resource/head/man.svg"));
        ui->labName->setStyleSheet("color: black;");
        ui->stackedWidget->setCurrentIndex(0);
    } else if (item->GetItemState() == ContactsState::Done) {
        ui->labName_2->setText(item->GetItemName());
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
    user.SetUserName(m_item->GetItemName().toStdString());
    user.SetUserId(m_item->GetItemId());
    emit SIG_AddFriendReqAck(user);
}