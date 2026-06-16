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
    QAction *action1 = menu->addAction("添加朋友");
    menu->addAction("选项2");
    connect(action1, &QAction::triggered, this, [=](){
        qDebug() << "添加朋友";
        emit SIG_AddFriend();
    });
    ui->btnPlus->setMenu(menu);



}

ChatPaneWidget::~ChatPaneWidget()
{
    delete ui;
}

void ChatPaneWidget::SlotReciveAddFriendReq(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");

    QJsonObject dataObj = dataVal.toObject();
    QString friendname = dataObj.value("sendername").toString();
    int friendid = dataObj.value("senderid").toInt();

    if (m_mapIdToChatItem.find(friendid) == m_mapIdToChatItem.end()) {
        QList<Message> messages1;
        Friend* fri = new Friend(friendid, friendname, ":/resource/head/man.svg", false, "2026", 0, messages1);

        auto item = new QListWidgetItem;
        QVariant var = QVariant::fromValue(fri);
        item->setData(Qt::UserRole, var);
        item->setSizeHint(QSize(250, 65));
        ui->listWidget->addItem(item);

        auto widget = new ChatListItem;
        widget->SetItem(fri);
        m_mapIdToChatItem[fri->GetUserId()] = widget;
        ui->listWidget->setItemWidget(item, widget);

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