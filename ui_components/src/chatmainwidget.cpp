#include "chatmainwidget.h"
#include "ui_chatmainwidget.h"
#include "chatlistitem.h"
#include "chatpanewidget.h"
#include "senderwidget.h"
#include "receiverwidget.h"
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

void ChatMainWidget::SetStackedWidgettCurrentIndex(int index) {
    ui->ChatMainStackedWidget->setCurrentIndex(index);
}


void ChatMainWidget::SetData(Friend *data) {
    ui->listWidget->clear();    // 清空原来的聊天记录
    qDebug() << "ChatMainWidget::SetData name: " << QString::fromStdString(data->GetUserName());
    ui->labName->setText(QString::fromStdString(data->GetUserName()));
    ui->labName->setStyleSheet("color: black;");

    auto messages = data->GetMessages();
    for (int i = 0; i < messages.count(); i++) {
        auto message = messages[i];
        qDebug() << "ChatMainWidget::SetData message: " << message.GetContent();
        auto item = new QListWidgetItem;
        ui->listWidget->addItem(item);
        if (message.GetMsgType() == MsgType::Sender) {
            auto widget = new SenderMsgItem(nullptr, ":/resource/head/man.svg");
            ui->listWidget->setItemWidget(item, widget);
            // connect(widget, &SenderWidget::SIG_LabelSizeChanged, [=](QRect rect) mutable {
            //     item->setSizeHint(QSize(width() * 3 / 5, rect.height() + 20));
            // });
            widget->SetMessage(message.GetContent());
            item->setSizeHint(widget->sizeHint());
            // 取消item可选中，彻底杜绝焦点框
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        } else if (message.GetMsgType() == MsgType::Receive) {
            auto widget = new ReceiverMsgItem(nullptr, ":/resource/icon/app.png");
            ui->listWidget->setItemWidget(item, widget);
            // connect(widget, &ReceiverWidget::SIG_LabelSizeChanged, [=](QRect rect) mutable {
            //     item->setSizeHint(QSize(width() * 3 / 5, rect.height() + 20));
            // });
            widget->SetMessage(message.GetContent());
            item->setSizeHint(widget->sizeHint());
            // 取消item可选中，彻底杜绝焦点框
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
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