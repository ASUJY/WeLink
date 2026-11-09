#include "chatwidget.h"
#include "ui_chatwidget.h"
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

    InitSearchLineEdit();

    qApp->installEventFilter(this);

}

void ChatWidget::InitSearchLineEdit() {
    QAction* searchAction = new QAction(ui->leditSearch);
    searchAction->setIcon(QIcon(":/resource/icon/menu/search2.svg"));
    ui->leditSearch->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->leditSearch->setPlaceholderText(tr("搜索"));

    // 创建一个清除动作并设置图标
    QAction *clearAction = new QAction(ui->leditSearch);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    // 初始时不显示清除图标
    // 将清除动作添加到LineEdit的末尾位置
    ui->leditSearch->addAction(clearAction, QLineEdit::TrailingPosition);

    // 当需要显示清除图标时，更改为实际的清除图标
    connect(ui->leditSearch, &QLineEdit::textChanged, [clearAction](const QString &text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/resource/icon/menu/close_search.png"));
        } else {
            clearAction->setIcon(QIcon(":/resource/icon/menu/close_transparent.png")); // 文本为空时，切换回透明图标
        }
    });

    // 连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction, &QAction::triggered, this, [this, clearAction]() {
        ui->leditSearch->clear();
        clearAction->setIcon(QIcon(":/resource/icon/menu/close_transparent.png")); // 清除文本后，切换回透明图标
        ui->leditSearch->clearFocus();
        //清除按钮被按下则不显示搜索框
        // ShowSearch(false);
    });

    InitSearchPopup();
}

void ChatWidget::InitSearchPopup() {
    m_searchPopup = new SearchPopup(this);

    // 点击搜索框时显示弹窗
    connect(ui->leditSearch, &SearchLineEdit::SIG_FOCUS_IN, this, [this]() {
        m_searchPopup->ShowBelow(ui->leditSearch);
    });

    connect(ui->leditSearch, &QLineEdit::textChanged, this, [this](const QString& text) {
        m_searchPopup->SetItemText(text);
        if (!m_searchPopup->isVisible()) {
            m_searchPopup->ShowBelow(ui->leditSearch);
        }
    });

    connect(m_searchPopup, &SearchPopup::SIG_AddFriendClicked, this, &ChatWidget::SIG_AddFriendPopup);

    // 输入文字时显示弹窗
    // connect(ui->leditSearch, &SearchLineEdit::textChanged, this, [this](const QString &text) {
    //     if (!m_searchPopup.isVisible()) {
    //         m_searchPopup.ShowBelow(ui->leditSearch);
    //     }
    // });
}

bool ChatWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        HandleGlobalMousePress(mouseEvent);
    }
    return QWidget::eventFilter(watched, event);
}

void ChatWidget::HandleGlobalMousePress(QMouseEvent *event) {

    if (!m_searchPopup->isVisible()) {
        return;
    }
    // 鼠标点击的全局坐标
    const QPoint globalPos = event->globalPosition().toPoint();

    // 搜索框在屏幕坐标中的矩形
    const QRect searchRect(ui->leditSearch->mapToGlobal(QPoint(0, 0)),
        ui->leditSearch->size());

    // 弹窗在屏幕坐标中的矩形
    const QRect popupRect(m_searchPopup->mapToGlobal(QPoint(0, 0)),
        m_searchPopup->size());


    // 点击搜索框或者弹窗内部，不关闭
    // 如果点击位置不在弹窗内部，则关闭弹窗
    if (searchRect.contains(globalPos) || popupRect.contains(globalPos))
    {
        return;
    }

    ui->leditSearch->clear();
    m_searchPopup->HidePopup();
}

ChatWidget::~ChatWidget()
{
    qApp->removeEventFilter(this);
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

    ChatListItem* chatItem = new ChatListItem(this);
    chatItem->SetItem(std::move(fri));

    QListWidgetItem* listItem  = new QListWidgetItem;       // 创建列表单行载体对象，代表列表中的一行。
    // QVariant var = QVariant::fromValue(fri.get());
    // listItem->setData(Qt::UserRole, var);
    listItem->setSizeHint(chatItem->sizeHint());
    ui->chatpaneListWidget->addItem(listItem );                     // 在列表中新增一行
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
    ui->chatListView->SetHeader(":/resource/head/man.svg", ":/resource/icon/app.png");
    ui->labName->setText(
        QString::fromStdString(data->GetUserName()));

    QList<ChatMessage> rows;
    const auto &messages = data->GetMessages();

    for (const auto &message : messages) {
        ChatMessage row;

        row.text = message.GetContent();

        row.msgType =
            message.GetMsgType() == MsgType::Sender
                ? ChatMessageType::Sender
                : ChatMessageType::Receiver;

        // 将此处替换为 Message 的实际时间读取接口。
        // 如果 GetTime() 返回 std::string：
        // row.time = QString::fromStdString(message.GetTime().toStdString());
        // 如果返回 QString：
        row.time = message.GetTime();

        // 将此处替换为 Message 的实际图片字段或图片路径接口。
        // row.imagePath = message.GetImagePath();

        row.contentType = row.imagePath.isEmpty()
                              ? ChatContentType::Text
                              : ChatContentType::Image;

        rows.append(row);
    }

    ui->chatListView->SetMessage(rows);
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