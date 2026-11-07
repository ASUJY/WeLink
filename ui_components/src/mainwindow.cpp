#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "friend.hpp"

MainWindow::MainWindow(std::shared_ptr<User> user, std::shared_ptr<FriendModel> friendModel,
                       std::shared_ptr<MsgModel> msgModel, std::shared_ptr<FriendRequestModel> friendRequestModel,
                       QWidget *parent) noexcept
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_user(user),
    m_friendModel(friendModel),
    m_friendRequestModel(friendRequestModel),
    m_msgModel(msgModel)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    // this->setWindowIcon(QIcon{});
    this->setMouseTracking(true);   // 开启鼠标跟踪，让窗口在鼠标未按下时也能接收 mouseMoveEvent 事件
    // SetWidgetWinTitle();
    // Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init(const std::shared_ptr<User>& user) {
    InitChatWidget();
    InitContactsWidget(user);

    // this->setMouseTracking(true);
    // this->installEventFilter(this); // 将事件过滤器对象(this)安装到目标对象(this)上
    m_btn = ui->btnAllChat;
    connect(ui->btnAllChat, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
    connect(ui->btnContact, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
}

void MainWindow::InitAddFriendWindow() {
    if (m_addFriendWindow)
        return;

    m_addFriendWindow = new AddFriendWindow(this);
    connect(m_addFriendWindow, &QObject::destroyed, this, [this](){
        m_addFriendWindow = nullptr;
    });
    m_addFriendWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_GetFriendInfo,
            this, &MainWindow::SIG_SEND_GetFriendInfo);
    connect(this, static_cast<void (MainWindow::*)(const QByteArray&)>(&MainWindow::SIG_RECEIVE_GetFriendInfoACK), m_addFriendWindow,
            static_cast<void(AddFriendWindow::*)(const QByteArray&)>(&AddFriendWindow::ReceiveSlotGetFriendInfoACK));
    connect(this, static_cast<void (MainWindow::*)(const User& user)>(&MainWindow::SIG_RECEIVE_GetFriendInfoACK),
            m_addFriendWindow, static_cast<void(AddFriendWindow::*)(const User&)>(&AddFriendWindow::ReceiveSlotGetFriendInfoACK));

    // 主动发送添加好友请求
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_AddFriendReq, this, &MainWindow::SendSlotAddFriendReq);
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_AddFriendReq, m_contactsWidget, &ContactsWidget::SlotAddFriendReq);
}

void MainWindow::InitChatWidget() {
    m_chatWidget = new ChatWidget(this);
    // 主动发送消息
    connect(m_chatWidget, &ChatWidget::SIG_SendChatMsg, this, &MainWindow::SlotSendChatMsg);

    // 添加好友按钮
    connect(m_chatWidget, &ChatWidget::SIG_AddFriend, this, &MainWindow::ShowAddFriendWindow);
    // 选择聊天列表中的某一项，展示对应的聊天窗口
    connect(m_chatWidget, &ChatWidget::SIG_ItemClicked, this, &MainWindow::SlotChatView);
    // 接收好友添加请求，在聊天列表中增加一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendAck, m_chatWidget, &ChatWidget::SlotReciveAddFriendAckAgree);

    auto friends = m_friendModel->FindFriends(m_user->GetUserId());
    for (int i = 0; i < friends.size(); ++i) {
        QList<Message> emptyMsg;
        std::unique_ptr<Friend> fri = std::make_unique<Friend>(friends[i].GetUserId(), QString::fromStdString(friends[i].GetUserName()), ":/resource/head/man.svg", false, "2026", 0, emptyMsg);
        auto msgVec = m_msgModel->FindMsg(m_user->GetUserId(), friends[i].GetUserId());
        for (int j = 0; j < msgVec.size(); ++j) {
            fri->AddMessage(msgVec[j]);
        }
        m_chatWidget->AddFriendToPane(std::move(fri));
    }

    ui->stackedWidget->addWidget(m_chatWidget);
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::InitContactsWidget(const std::shared_ptr<User>& user) {
    m_contactsWidget = new ContactsWidget(user, m_friendModel, m_friendRequestModel, this);
    // 接收到好友发来的添加朋友请求，在 新的好友列表 中新增一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendReq, m_contactsWidget, &ContactsWidget::ReceiveSlotAddFriendReq);
    // 接收到好友发来的添加朋友通过请求，在 联系人列表 中新增一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendAck, m_contactsWidget, &ContactsWidget::SlotReciveAddFriendAckAgree);
    // 联系人列表中的某一项被选中，则展示对应的内容
    connect(m_contactsWidget, &ContactsWidget::SIG_ItemDidSelected, this, &MainWindow::SlotContactsItemDidSelected);

    connect(m_contactsWidget, &ContactsWidget::SIG_SEND_AddFriendReqAck, this, &MainWindow::SendSlotAddFriendReqAck);
    // connect(this, &MainWindow::SIG_ReciveAddFriendReq, m_chatPaneWidget, &ChatPaneWidget::SlotReciveAddFriendReq);

    auto friends = m_friendModel->FindFriends(m_user->GetUserId());
    for (int i = 0; i < friends.size(); ++i) {
        m_contactsWidget->AddFriendToPane(friends[i]);
    }

    auto friendsrequests = m_friendRequestModel->GetAllFriendRequest(m_user->GetUserId());
    for (auto it = friendsrequests.begin(); it != friendsrequests.end(); ++it) {
        m_contactsWidget->AddFriendToPane(*it);
    }

    ui->stackedWidget->addWidget(m_contactsWidget);
}


// void MainWindow::SetWidgetWinTitle() {
//     ui->winTitleWidget->setFixedHeight(24);

//     QSize btnSize(12,12);
//     ui->btnMWinClose->setFixedSize(btnSize);
//     ui->btnMWinMin->setFixedSize(btnSize);
//     ui->btnMWinMax->setFixedSize(btnSize);

//     ui->btnMWinClose->setFlat(true);
//     ui->btnMWinMin->setFlat(true);
//     ui->btnMWinMax->setFlat(true);

//     connect(ui->btnMWinClose, &QPushButton::clicked, this, [=](){
//         this->close();
//     });
//     connect(ui->btnMWinMin, &QPushButton::clicked, this, [=](){
//         this->showMinimized();
//     });
//     connect(ui->btnMWinMax, &QPushButton::clicked, this, [=](){
//         this->showMaximized();
//     });
// }

/**
 * 根据鼠标坐标(x,y)判断鼠标位于窗口的哪个区域
 */
Area MainWindow::GetArea(int x, int y) {
    auto rect = geometry();             // 获取当前窗口的几何信息(位置和尺寸)
    constexpr int resizemargin = 10;    // 距离窗口边缘10像素内视为边缘区域
    if (x <= resizemargin) {            // 左侧边缘
        if (y >= rect.height() - resizemargin) {
            return Area::BottomLeft;
        } else if (y <= resizemargin) {
            return Area::TopLeft;
        } else {
            return Area::Left;
        }
    } else if (x >= rect.width() - resizemargin) {  // 右侧边缘
        if (y >= rect.height() - resizemargin) {
            return Area::BottomRight;
        } else if (y <= resizemargin) {
            return Area::TopRight;
        } else {
            return Area::Right;
        }
    } else {
        if (y >= rect.height() - resizemargin) {
            return Area::Bottom;
        } else if (y <= resizemargin) {
            return Area::Top;
        }
    }
    return Area::Center;
}

/**
 * 根据区域返回对应的光标形状
 */
Qt::CursorShape MainWindow::GetCursorForArea(Area area) {
    switch (area) {
        case Area::Top: case Area::Bottom:          return Qt::SizeVerCursor;
        case Area::Left: case Area::Right:          return Qt::SizeHorCursor;
        case Area::TopLeft: case Area::BottomRight: return Qt::SizeFDiagCursor;
        case Area::TopRight: case Area::BottomLeft: return Qt::SizeBDiagCursor;
        case Area::Center:                          return Qt::SizeAllCursor;
        default:                                    return Qt::ArrowCursor;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isMouseDown = true;
        m_pressRect = geometry();                       // 窗口的当前位置和尺寸(左上角坐标和宽高)
        m_pressPos = event->globalPosition().toPoint(); // 鼠标按下时的全局位置
        m_posx = mapFromGlobal(m_pressPos).x();         // 将全局坐标转换为相当于当前窗口左上角的局部坐标
        m_posy = mapFromGlobal(m_pressPos).y();
        m_area = GetArea(event->pos().x(), event->pos().y());
        setCursor(GetCursorForArea(m_area));
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!m_isMouseDown) return;

    auto currentPos = event->globalPosition().toPoint();
    auto moveSize = currentPos - m_pressPos;
    auto currentRect = m_pressRect;

    setCursor(GetCursorForArea(m_area));

    switch (m_area) {
    case Area::Top:
        currentRect.setTop(m_pressRect.top() + moveSize.y());
        break;
    case Area::Bottom:
        currentRect.setBottom(m_pressRect.bottom() + moveSize.y());
        break;
    case Area::Left:
        currentRect.setLeft(m_pressRect.left() + moveSize.x());
        break;
    case Area::Right:
        currentRect.setRight(m_pressRect.right() + moveSize.x());
        break;
    case Area::TopLeft:
        currentRect.setTopLeft(m_pressRect.topLeft() + moveSize);
        break;
    case Area::TopRight:
        currentRect.setTopRight(m_pressRect.topRight() + moveSize);
        break;
    case Area::BottomLeft:
        currentRect.setBottomLeft(m_pressRect.bottomLeft() + moveSize);
        break;
    case Area::BottomRight:
        currentRect.setBottomRight(m_pressRect.bottomRight() + moveSize);
        break;
    case Area::Center:
        move(currentPos.x() - m_posx, currentPos.y() - m_posy);
        return;
    }

    const int minW = 700;
    const int minH = 500;
    bool needAdjust = false;
    if (currentRect.width() < minW) {
        needAdjust = true;
        if (currentRect.left() != m_pressRect.left()) {
            currentRect.setLeft(currentRect.right() - minW);
        } else {
            currentRect.setRight(currentRect.left() + minW);
        }
    }
    if (currentRect.height() < minH) {
        needAdjust = true;
        if (currentRect.top() != m_pressRect.top()) {
            currentRect.setTop(currentRect.bottom() - minH);
        } else {
            currentRect.setBottom(currentRect.top() + minH);
        }
    }
    if (needAdjust || currentRect != geometry()) {
        setGeometry(currentRect);
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    m_isMouseDown = false;
    unsetCursor();
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::ShowAddFriendWindow() {
    InitAddFriendWindow();
    if (m_addFriendWindow) {
        m_addFriendWindow->show();
    }
}

void MainWindow::ReceiveSlotGetFriendInfoACK(const QByteArray& data) {
    emit SIG_RECEIVE_GetFriendInfoACK(data);
}

void MainWindow::SlotGetFriendInfoFailed(const QByteArray& data) {
    // emit SIG_GetFriendInfoFailed(data);
}

void MainWindow::ReceiveSlotAddFriendReq(const QByteArray& data) {
    emit SIG_RECEIVE_AddFriendReq(data);
}

void MainWindow::ReceiveSlotAddFriendAck(const QByteArray& data) {
    emit SIG_RECEIVE_AddFriendAck(data);
}

void MainWindow::SlotSelectEvent() {
    // 取消左侧导航栏之前选中的按钮的选中状态，然后将当前的按钮设置为选中状态
    m_btn->setChecked(false);
    m_btn = qobject_cast<QPushButton*>(sender());
    m_btn->setChecked(true);

    auto name = m_btn->objectName();
    if (name == "btnAllChat") {
        ui->stackedWidget->setCurrentIndex(static_cast<int>(ChatPage::ChatWidget));
    } else {
        ui->stackedWidget->setCurrentIndex(static_cast<int>(ChatPage::Contactwidget));
    }

}

void MainWindow::SlotChatView(QVariant var, PageType type) {
    if (var.canConvert<Friend*>() && type == PageType::AllChatView) {
        auto data = var.value<Friend*>();
        m_chatWidget->SetStackedWidgetCurrentIndex(0);
        m_chatWidget->SetData(data);
    }
}

void MainWindow::SlotSendChatMsg(int64_t id, const QString& message) {
    qDebug() << "MainWindow::SlotSendChatMsg: userid" << id;
    emit SIG_SendChatMsg(id, message);
}

void MainWindow::SlotContactsItemDidSelected(const std::shared_ptr<ContactsItem>& item) {
    m_contactsWidget->SetStackedWidgetCurrentIndex(item);
}

QByteArray MainWindow::BuildAddFriendRequestPacket(const User& self, const User& target)
{
    QJsonObject dataJson;
    dataJson["friendname"] = QString::fromStdString(target.GetUserName());
    dataJson["friendid"] = target.GetUserId();
    dataJson["username"] = QString::fromStdString(self.GetUserName());
    dataJson["userid"] = self.GetUserId();

    QJsonObject root;
    root["data"] = dataJson;
    root["msgtype"] = static_cast<int>(E_MSG_TYPE::ADD_FRIEND_REQ);
    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray MainWindow::BuildAddFriendAckPacket(const User& self, const User& target, FriendState state)
{
    QJsonObject dataJson;
    dataJson["friendname"] = QString::fromStdString(target.GetUserName());
    dataJson["friendid"] = target.GetUserId();
    dataJson["username"] = QString::fromStdString(self.GetUserName());
    dataJson["userid"] = self.GetUserId();
    dataJson["ackType"] = static_cast<int>(state);

    QJsonObject root;
    root["data"] = dataJson;
    root["msgtype"] = static_cast<int>(E_MSG_TYPE::ADD_FRIEND_ACK);
    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

void MainWindow::SendSlotAddFriendReqAck(const User& frienduser, FriendState type) {
    qDebug() << "MainWindow::SlotAddFriendReqAck";
    if (type == FriendState::ACCEPT) {
        m_contactsWidget->SlotAddFriendReqAck(frienduser);  // 在联系人列表中添加朋友项
        // m_chatPaneWidget->SlotAddFriendReqAck(frienduser);      // 在聊天列表中添加朋友项
    }
    m_friendRequestModel->UpdateItemStatus(m_user->GetUserId(), frienduser.GetUserId(), static_cast<FriendState>(type));
    QByteArray pkt = BuildAddFriendAckPacket(*m_user, frienduser, type);

    emit SIG_SEND_AddFriendReqAck(pkt, frienduser, type);
}

void MainWindow::ReceiveSlotOneChat(const QByteArray& data) {

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError) || !jsonDoc.isObject()) {
        qWarning() << "SlotOneChat JSON parse error:" << jsonError.errorString();
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();
    qDebug() << "MainWindow::SlotOneChat" << dataObj.value("senderId");
    int64_t id = dataObj.value("senderId").toString().toLongLong();
    QString content = dataObj.value("content").toString();
    QString createtime = dataObj.value("createtime").toString();
    qDebug() << "MainWindow::SlotOneChat";
    qDebug() << "id: " << id << " content: " << content << " createtime: " << createtime;
    auto item = m_chatWidget->GetItemById(id);
    if (item) {
        if (item == m_chatWidget->GetCurrentSelectedItem()) {
            Message message(content, createtime, Receive);
            item->UpdateContent2(message);
            m_msgModel->AddMsg(m_user->GetUserId(), id, message);
            return;
        }
        Message message(content, createtime, Receive);
        item->UpdateContent(message);
        m_msgModel->AddMsg(m_user->GetUserId(), id, message);
    } else {
        qDebug() << "MainWindow::SlotOneChat failed";
    }
}

void MainWindow::SendSlotAddFriendReq(const User& frienduser) {
    qDebug() << frienduser.GetUserName() << "AppCore SlotAddFriendReq " << frienduser.GetUserId();

    QByteArray pkt = BuildAddFriendRequestPacket(*m_user, frienduser);
    emit SIG_SEND_AddFriendReq(pkt);
    m_friendRequestModel->AddItem(m_user->GetUserId(), frienduser, FriendState::PendingVerification);
}