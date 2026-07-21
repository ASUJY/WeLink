#include "chatpanewidget.h"
#include "ui_chatpanewidget.h"
#include <QToolButton>
#include <QMenu>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

#include "message.hpp"
#include "friend.hpp"

ChatListItem* ChatPaneWidget::m_selectedItem = nullptr;

ChatPaneWidget::ChatPaneWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPaneWidget)
{
    ui->setupUi(this);

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

ChatPaneWidget::~ChatPaneWidget()
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

void ChatPaneWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
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

void ChatPaneWidget::AddFriendToPane(std::unique_ptr<Friend>&& fri) {
    if (!fri)
        return;


    int64_t friendid = fri->GetUserId();
    if (m_mapIdToChatItem.find(friendid) != m_mapIdToChatItem.end()) {
        return;
    }
    CreateChatListItem(friendid, std::move(fri));
}

void ChatPaneWidget::CreateChatListItem(int64_t friendId, std::unique_ptr<Friend>&& fri) {

    QListWidgetItem* listItem  = new QListWidgetItem;       // 创建列表单行载体对象，代表列表中的一行。
    // QVariant var = QVariant::fromValue(fri.get());
    // listItem->setData(Qt::UserRole, var);
    listItem->setSizeHint(QSize(250, 65));
    ui->listWidget->addItem(listItem );                     // 在列表中新增一行

    ChatListItem* chatItem = new ChatListItem(this);
    chatItem->SetItem(std::move(fri));
    ui->listWidget->setItemWidget(listItem , chatItem);     // 把列表中的这一行的显示效果替换为自定义控件的显示效果

    m_mapIdToChatItem[friendId] = chatItem;
    connect(chatItem, &ChatListItem::SIG_Selected, this, &ChatPaneWidget::SlotItemSelected);
}

void ChatPaneWidget::SlotAddFriendReqAck(const User& user) {

    QString friendname = QString::fromStdString(user.GetUserName());
    int64_t friendid = user.GetUserId();

    if (m_mapIdToChatItem.find(friendid) != m_mapIdToChatItem.end()) {
        return;
    }

    QList<Message> emptyMsgList;
    std::unique_ptr<Friend> fri = std::make_unique<Friend>(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, emptyMsgList);

    CreateChatListItem(friendid, std::move(fri));

}

void ChatPaneWidget::SlotItemSelected(ChatListItem *item) {
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

ChatListItem* ChatPaneWidget::GetItemById(int64_t id) const{
    auto iter = m_mapIdToChatItem.find(id);
    if (iter == m_mapIdToChatItem.cend())
        return nullptr;
    return iter->second;
}