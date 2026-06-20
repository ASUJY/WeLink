#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"

ContactsMainWidget::ContactsMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
}

ContactsMainWidget::~ContactsMainWidget()
{
    delete ui;
}
