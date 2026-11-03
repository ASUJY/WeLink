#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "ReceiverMsgItem.h"
#include "SenderMsgItem.h"
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>

ChatListItem* ChatWidget::m_selectedItem = nullptr;

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    // chatmain init
    ui->ChatMainStackedWidget->setCurrentIndex(1);
    connect(ui->btnSendMsg, &QPushButton::clicked, this, &ChatWidget::SlotBtnSendMsgClicked);


    // chantpane init
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    ui->btnPlus->setPopupMode(QToolButton::InstantPopup);
    ui->btnPlus->setStyleSheet("QToolButton:menu-indicator {image: none;}");

    QMenu *menu = new QMenu(this);
    QAction *addFriendAction = menu->addAction("添加朋友");
    menu->addAction("选项2");

    connect(addFriendAction, &QAction::triggered, this, [=](){
        qDebug() << "触发添加朋友菜单";
        emit SIG_AddFriend();
    });
    ui->btnPlus->setMenu(menu);
}

ChatWidget::~ChatWidget()
{
    for (auto it = m_mapIdToChatItem.begin(); it != m_mapIdToChatItem.end(); ++it) {
        ChatListItem* widget = it->second;
        if (widget) {
            widget->deleteLater();
        }
    }
    m_mapIdToChatItem.clear();

    delete ui;
}

void ChatWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError) || !jsonDoc.isObject()) {
        qWarning() << "解析新增好友ACK数据包失败:" << jsonError.errorString();
        return;
    }

    QJsonObject rootObj  = jsonDoc.object();
    QJsonValue dataVal = rootObj .value("data");
    QJsonObject dataObj = dataVal.toObject();

    QString friendname = dataObj["sendername"].toString();
    int64_t friendid = dataObj["senderid"].toInteger();

    qDebug() << "ChatPaneWidget::SlotReciveAddFriendAckAgree: " << "friendid: " << friendid;

    if (m_mapIdToChatItem.find(friendid) != m_mapIdToChatItem.end()) {
        return;
    }

    QList<Message> emptyMsgList;
    std::unique_ptr<Friend> fri = std::make_unique<Friend>(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, emptyMsgList);

    CreateChatListItem(friendid, std::move(fri));
}

void ChatWidget::AddFriendToPane(std::unique_ptr<Friend>&& fri) {
    if (!fri)
        return;


    int64_t friendid = fri->GetUserId();
    if (m_mapIdToChatItem.find(friendid) != m_mapIdToChatItem.end()) {
        return;
    }
    CreateChatListItem(friendid, std::move(fri));
}

void ChatWidget::CreateChatListItem(int64_t friendId, std::unique_ptr<Friend>&& fri) {

    QListWidgetItem* listItem  = new QListWidgetItem;       // 创建列表单行载体对象，代表列表中的一行。
    // QVariant var = QVariant::fromValue(fri.get());
    // listItem->setData(Qt::UserRole, var);
    listItem->setSizeHint(QSize(250, 65));
    ui->chatpaneListWidget->addItem(listItem );                     // 在列表中新增一行

    ChatListItem* chatItem = new ChatListItem(this);
    chatItem->SetItem(std::move(fri));
    ui->chatpaneListWidget->setItemWidget(listItem , chatItem);     // 把列表中的这一行的显示效果替换为自定义控件的显示效果

    m_mapIdToChatItem[friendId] = chatItem;
    connect(chatItem, &ChatListItem::SIG_Selected, this, &ChatWidget::SlotItemSelected);
}

void ChatWidget::SlotAddFriendReqAck(const User& user) {

    QString friendname = QString::fromStdString(user.GetUserName());
    int64_t friendid = user.GetUserId();

    if (m_mapIdToChatItem.find(friendid) != m_mapIdToChatItem.end()) {
        return;
    }

    QList<Message> emptyMsgList;
    std::unique_ptr<Friend> fri = std::make_unique<Friend>(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, emptyMsgList);

    CreateChatListItem(friendid, std::move(fri));

}

void ChatWidget::SlotItemSelected(ChatListItem *item) {
    if (item == nullptr) return;

    if (m_selectedItem != nullptr) {
        m_selectedItem->Selected(false);
        m_selectedItem->Checked(false);
    }
    item->Selected(true);
    item->Checked(true);
    m_selectedItem = item;

    QVariant var = QVariant::fromValue(item->GetItem());
    emit SIG_ItemClicked(var, PageType::AllChatView);
}

ChatListItem* ChatWidget::GetItemById(int64_t id) const{
    auto iter = m_mapIdToChatItem.find(id);
    if (iter == m_mapIdToChatItem.cend())
        return nullptr;
    return iter->second;
}



void ChatWidget::SetStackedWidgetCurrentIndex(int index) {
    ui->ChatMainStackedWidget->setCurrentIndex(index);
}

void ChatWidget::SetData(Friend *data) {

    if (!data) {
        qWarning() << "ChatWidget::SetData Friend 空指针";
        return;
    }

    ui->chatListWidget->clear();    // 清空原来的聊天记录
    qDebug() << "ChatWidget::SetData name: " << QString::fromStdString(data->GetUserName());
    ui->labName->setText(QString::fromStdString(data->GetUserName()));
    ui->labName->setStyleSheet("color: black;");

    const auto& messages = data->GetMessages();
    for (int i = 0; i < messages.count(); i++) {
        const auto& message = messages[i];
        qDebug() << "ChatWidget::SetData message: " << message.GetContent();

        auto item = new QListWidgetItem;
        ui->chatListWidget->addItem(item);

        // 取消item可选中，去除焦点虚线框
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        if (message.GetMsgType() == MsgType::Sender) {
            auto widget = new SenderMsgItem(nullptr, ":/resource/head/man.svg");
            widget->SetMessage(message.GetContent());

            ui->chatListWidget->setItemWidget(item, widget);
            item->setSizeHint(widget->sizeHint());

        } else if (message.GetMsgType() == MsgType::Receive) {
            auto widget = new ReceiverMsgItem(nullptr, ":/resource/icon/app.png");
            widget->SetMessage(message.GetContent());

            ui->chatListWidget->setItemWidget(item, widget);
            item->setSizeHint(widget->sizeHint());
        }
    }
    ui->chatListWidget->scrollToBottom();
}

void ChatWidget::SlotBtnSendMsgClicked() {
    QString content = ui->textEdit->toPlainText().trimmed();
    if (content.isEmpty()) return;

    ChatListItem* chatItem = ChatWidget::GetCurrentSelectedItem();
    if (!chatItem || !chatItem->GetItem()) {
        qWarning() << "未选中任何聊天对象，无法发送消息";
        return;
    }

    Message message(content, QTime::currentTime().toString("hh:mm:ss"), Sender);
    chatItem->UpdateFriend(message);

    //
    qDebug() << "ChatWidget::SlotBtnSendMsgClicked: userid: " << chatItem->GetItem()->GetUserId();
    emit SIG_SendChatMsg(chatItem->GetItem()->GetUserId(), content);
    ui->textEdit->clear();
    SetData(chatItem->GetItem());
}