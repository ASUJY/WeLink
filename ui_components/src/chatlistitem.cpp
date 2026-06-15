#include "chatlistitem.h"
#include "ui_chatlistitem.h"

ChatListItem::ChatListItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatListItem)
{
    ui->setupUi(this);
}

ChatListItem::~ChatListItem()
{
    delete ui;
}

void ChatListItem::SetItem(Friend *data) {
    if (data == nullptr) return;

    m_data = data;
    auto pixmap = QPixmap(":/resource/head/man.svg");
    ui->labHeadIcon->setPixmap(pixmap);
    ui->labName->setText(QString::fromStdString(data->GetUserName()));
    ui->labTime->setText(data->GetTime());
    if (data->GetMessages().count() > 0) {
        // setContent(data)
    }
    ui->labMsgCount->setText(QString::number(data->GetCount()));
    ui->chatlistitem_widget->setVisible(data->GetCount() != 0);
    ui->labMute->setVisible(data->GetMute());
}