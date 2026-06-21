#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"

ContactsMainWidget::ContactsMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3);
}

ContactsMainWidget::~ContactsMainWidget()
{
    delete ui;
}

void ContactsMainWidget::SetStackedWidgetCurrentIndex(ContactsListViewChild *item) {
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