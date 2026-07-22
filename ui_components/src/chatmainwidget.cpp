#include "chatmainwidget.h"
#include "ui_chatmainwidget.h"
#include "chatlistitem.h"
#include "chatpanewidget.h"
#include "ReceiverMsgItem.h"
#include "SenderMsgItem.h"
#include <QDebug>

ChatMainWidget::ChatMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatMainWidget)
{
    ui->setupUi(this);
    ui->ChatMainStackedWidget->setCurrentIndex(1);
    connect(ui->btnSendMsg, &QPushButton::clicked, this, &ChatMainWidget::SlotBtnSendMsgClicked);
}

ChatMainWidget::~ChatMainWidget()
{
    delete ui;
}

void ChatMainWidget::SetStackedWidgetCurrentIndex(int index) {
    ui->ChatMainStackedWidget->setCurrentIndex(index);
}


void ChatMainWidget::SetData(Friend *data) {

    if (!data) {
        qWarning() << "ChatMainWidget::SetData Friend 空指针";
        return;
    }

    ui->listWidget->clear();    // 清空原来的聊天记录
    qDebug() << "ChatMainWidget::SetData name: " << QString::fromStdString(data->GetUserName());
    ui->labName->setText(QString::fromStdString(data->GetUserName()));
    ui->labName->setStyleSheet("color: black;");

    const auto& messages = data->GetMessages();
    for (int i = 0; i < messages.count(); i++) {
        const auto& message = messages[i];
        qDebug() << "ChatMainWidget::SetData message: " << message.GetContent();

        auto item = new QListWidgetItem;
        ui->listWidget->addItem(item);

        // 取消item可选中，去除焦点虚线框
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        if (message.GetMsgType() == MsgType::Sender) {
            auto widget = new SenderMsgItem(nullptr, ":/resource/head/man.svg");
            widget->SetMessage(message.GetContent());

            ui->listWidget->setItemWidget(item, widget);
            item->setSizeHint(widget->sizeHint());

        } else if (message.GetMsgType() == MsgType::Receive) {
            auto widget = new ReceiverMsgItem(nullptr, ":/resource/icon/app.png");
            widget->SetMessage(message.GetContent());

            ui->listWidget->setItemWidget(item, widget);
            item->setSizeHint(widget->sizeHint());
        }
    }
    ui->listWidget->scrollToBottom();
}

void ChatMainWidget::SlotBtnSendMsgClicked() {
    QString content = ui->textEdit->toPlainText().trimmed();
    if (content.isEmpty()) return;

    ChatListItem* chatItem = ChatPaneWidget::GetCurrentSelectedItem();
    if (!chatItem || !chatItem->GetItem()) {
        qWarning() << "未选中任何聊天对象，无法发送消息";
        return;
    }

    Message message(content, QTime::currentTime().toString("hh:mm:ss"), Sender);
    chatItem->UpdateFriend(message);

    //
    qDebug() << "ChatMainWidget::SlotBtnSendMsgClicked: userid: " << chatItem->GetItem()->GetUserId();
    emit SIG_SendChatMsg(chatItem->GetItem()->GetUserId(), content);
    ui->textEdit->clear();
    SetData(chatItem->GetItem());
}