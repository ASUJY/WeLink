#include "chatmainwidget.h"
#include "ui_chatmainwidget.h"
#include "chatlistitem.h"
#include "chatpanewidget.h"
#include "senderwidget.h"
#include <QDebug>

ChatMainWidget::ChatMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatMainWidget)
{
    ui->setupUi(this);
    connect(ui->btnSendMsg, &QPushButton::clicked, this, &ChatMainWidget::SlotBtnSendMsgClicked);
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
    for (int i = 0; i < messages.count(); i++) {
        auto message = messages[i];
        qDebug() << "message: " << message.GetContent();
        auto item = new QListWidgetItem;
        ui->listWidget->addItem(item);
        if (message.GetMsgType() == MsgType::Sender) {
            auto widget = new SenderWidget(nullptr, ":/resource/head/man.svg");
            ui->listWidget->setItemWidget(item, widget);
            connect(widget, &SenderWidget::SIG_LabelSizeChanged, [=](QRect rect) mutable {
                item->setSizeHint(QSize(width() * 3 / 5, rect.height() + 20));
            });
            widget->SetMessage(message.GetContent());
        } else if (message.GetMsgType() == MsgType::Receive) {
            auto widget = new SenderWidget(nullptr, ":/resource/head/man.svg");
            ui->listWidget->setItemWidget(item, widget);
            connect(widget, &SenderWidget::SIG_LabelSizeChanged, [=](QRect rect) mutable {
                item->setSizeHint(QSize(width() * 3 / 5, rect.height() + 20));
            });
        }
        ui->listWidget->scrollToBottom();
    }
}

void ChatMainWidget::SlotBtnSendMsgClicked() {
    QString content = ui->textEdit->toPlainText();
    if (content.isEmpty()) return;

    ChatListItem* chatitem = ChatPaneWidget::GetItem();
    Message message(content, QTime::currentTime().toString("hh:mm:ss"), Sender);
    chatitem->UpdateFriend(message);

    //
    qDebug() << "ChatMainWidget::SlotBtnSendMsgClicked: userid: " << chatitem->GetItem()->GetUserId();
    emit SIG_SendChatMsg(chatitem->GetItem()->GetUserId(), content);
    ui->textEdit->clear();
    SetData(chatitem->GetItem());
}