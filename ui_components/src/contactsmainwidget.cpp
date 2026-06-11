#include "contactsmainwidget.h"
#include "ui_contactsmainwidget.h"

ContactsMainWidget::ContactsMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsMainWidget)
{
    ui->setupUi(this);
}

ContactsMainWidget::~ContactsMainWidget()
{
    delete ui;
}
