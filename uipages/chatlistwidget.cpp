#include "chatlistwidget.h"
#include "ui_chatlistwidget.h"

ChatListWidget::ChatListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatListWidget)
{
    ui->setupUi(this);
}

ChatListWidget::~ChatListWidget()
{
    delete ui;
}
