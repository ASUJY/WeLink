#include "chatmainwidget.h"
#include "ui_chatmainwidget.h"

ChatMainWidget::ChatMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatMainWidget)
{
    ui->setupUi(this);
}

ChatMainWidget::~ChatMainWidget()
{
    delete ui;
}


void ChatMainWidget::SetData(Friend *data) {
    ui->listWidget->clear();    // 清空原来的聊天记录
    qDebug() << "name: " << QString::fromStdString(data->GetUserName());
    ui->labName->setText(QString::fromStdString(data->GetUserName()));
    auto messages = data->GetMessages();
}