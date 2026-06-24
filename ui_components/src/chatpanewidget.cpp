#include "chatpanewidget.h"
#include "ui_chatpanewidget.h"
#include <QToolButton>
#include <QMenu>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

#include "message.hpp"
#include "friend.hpp"

ChatListItem* ChatPaneWidget::m_item = nullptr;

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
        qDebug() << "添加朋友";
        emit SIG_AddFriend();
    });
    ui->btnPlus->setMenu(menu);
}

ChatPaneWidget::~ChatPaneWidget()
{
    delete ui;
}

void ChatPaneWidget::SlotReciveAddFriendAckAgree(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");

    QJsonObject dataObj = dataVal.toObject();
    QString friendname = dataObj.value("sendername").toString();
    uint64_t friendid = dataObj.value("senderid").toInteger();
    qDebug() << "ChatPaneWidget::SlotReciveAddFriendAckAgree: " << "friendid: " << friendid;
    if (m_mapIdToChatItem.find(friendid) == m_mapIdToChatItem.end()) {
        QList<Message> messages1;
        std::unique_ptr<Friend> fri = std::make_unique<Friend>(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, messages1);

        // 创建列表单行载体对象，代表列表中的一行。
        auto item = new QListWidgetItem;
        // QVariant var = QVariant::fromValue(fri.get());
        // item->setData(Qt::UserRole, var);
        item->setSizeHint(QSize(250, 65));
        // 在列表中新增一行
        ui->listWidget->addItem(item);
        auto widget = new ChatListItem(this);
        widget->SetItem(std::move(fri));
        // 把列表中的这一行的显示效果替换为自定义控件的显示效果
        ui->listWidget->setItemWidget(item, widget);

        m_mapIdToChatItem[friendid] = widget;

        connect(widget, &ChatListItem::SIG_Selected, this, &ChatPaneWidget::SlotItemSelected);
    }
}

void ChatPaneWidget::SlotAddFriendReqAck(const User& user) {

    QString friendname = QString::fromStdString(user.GetUserName());
    int friendid = user.GetUserId();

    if (m_mapIdToChatItem.find(friendid) == m_mapIdToChatItem.end()) {
        QList<Message> messages1;
        qDebug() << "ChatPaneWidget::SlotAddFriendReqAck: " << "friendid: " << friendid;
        std::unique_ptr<Friend> fri = std::make_unique<Friend>(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, messages1);

        auto item = new QListWidgetItem;
        // QVariant var = QVariant::fromValue(fri);
        // item->setData(Qt::UserRole, var);
        item->setSizeHint(QSize(250, 65));
        ui->listWidget->addItem(item);

        auto widget = new ChatListItem(this);
        widget->SetItem(std::move(fri));
        ui->listWidget->setItemWidget(item, widget);

        m_mapIdToChatItem[friendid] = widget;

        connect(widget, &ChatListItem::SIG_Selected, this, &ChatPaneWidget::SlotItemSelected);
    }
}

void ChatPaneWidget::SlotItemSelected(ChatListItem *item) {
    if (item == nullptr) return;
    if (m_item != nullptr) {
        m_item->Selected(false);
        m_item->Checked(false);
    }
    item->Selected(true);
    item->Checked(true);
    m_item = item;

    QVariant var = QVariant::fromValue(item->GetItem());
    emit SIG_ItemClicked(var, PageType::AllChatView);
}

ChatListItem* ChatPaneWidget::GetItemById(uint64_t id) {
    if (m_mapIdToChatItem.find(id) == m_mapIdToChatItem.end()) {
        return nullptr;
    }
    return m_mapIdToChatItem[id];
}