#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "friend.hpp"

MainWindow::MainWindow(std::shared_ptr<User> user, std::shared_ptr<FriendModel> friendModel, std::shared_ptr<MsgModel> msgModel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_page(ChatWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    SetWidgetWinTitle();
    m_friendModel = friendModel;
    m_msgModel = msgModel;
    m_user = user;
    // Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init() {
    // InitAddFriendWindow();
    InitChatMainWidget();
    InitChatPaneWidget();
    InitContactsPaneWidget();
    InitContactsMainWidget();

    // this->setMouseTracking(true);
    // this->installEventFilter(this); // 将事件过滤器对象(this)安装到目标对象(this)上
    m_btn = ui->btnAllChat;
    connect(ui->btnAllChat, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
    connect(ui->btnContact, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
}

void MainWindow::InitAddFriendWindow() {
    m_addFriendWindow = new AddFriendWindow(this);
    m_addFriendWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_GetFriendInfo, this, &MainWindow::SIG_SEND_GetFriendInfo);
    connect(this, static_cast<void (MainWindow::*)(const QByteArray&)>(&MainWindow::SIG_RECEIVE_GetFriendInfoACK), m_addFriendWindow, static_cast<void(AddFriendWindow::*)(const QByteArray&)>(&AddFriendWindow::ReceiveSlotGetFriendInfoACK));
    connect(this, static_cast<void (MainWindow::*)(const User& user)>(&MainWindow::SIG_RECEIVE_GetFriendInfoACK), m_addFriendWindow, static_cast<void(AddFriendWindow::*)(const User&)>(&AddFriendWindow::ReceiveSlotGetFriendInfoACK));

    // 主动发送添加好友请求
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_AddFriendReq, this, &MainWindow::SendSlotAddFriendReq);
    connect(m_addFriendWindow, &AddFriendWindow::SIG_SEND_AddFriendReq, m_contactsPaneWidget.get(), &ContactsPaneWidget::SlotAddFriendReq);
}

void MainWindow::InitChatMainWidget() {
    m_chatMainWidget = std::make_unique<ChatMainWidget>();
    // 主动发送消息
    connect(m_chatMainWidget.get(), &ChatMainWidget::SIG_SendChatMsg, this, &MainWindow::SlotSendChatMsg);
}

void MainWindow::InitChatPaneWidget() {
    m_chatPaneWidget = std::make_unique<ChatPaneWidget>();
    // 添加好友按钮
    connect(m_chatPaneWidget.get(), &ChatPaneWidget::SIG_AddFriend, this, &MainWindow::ShowAddFriendWindow);
    // 选择聊天列表中的某一项，展示对应的聊天窗口
    connect(m_chatPaneWidget.get(), &ChatPaneWidget::SIG_ItemClicked, this, &MainWindow::SlotChatView);
    // 接收好友添加请求，在聊天列表中增加一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendAck, m_chatPaneWidget.get(), &ChatPaneWidget::SlotReciveAddFriendAckAgree);

    auto friends = m_friendModel->FindFriends(m_user->GetUserId());
    for (int i = 0; i < friends.size(); ++i) {
        QList<Message> messages1;
        std::unique_ptr<Friend> fri = std::make_unique<Friend>(friends[i].GetUserId(), QString::fromStdString(friends[i].GetUserName()), ":/resource/head/man.svg", false, "2026", 0, messages1);
        auto msgVec = m_msgModel->FindMsg(m_user->GetUserId(), friends[i].GetUserId());
        for (int j = 0; j < msgVec.size(); ++j) {
            fri->AddMessage(msgVec[j]);
        }
        m_chatPaneWidget->AddFriendToPane(std::move(fri));
    }

    ui->gridLayout_2->addWidget(m_chatPaneWidget.get());
    m_chatPaneWidget->show();
}

void MainWindow::InitContactsPaneWidget() {
    m_contactsPaneWidget = std::make_unique<ContactsPaneWidget>(m_friendModel);
    // 接收到好友发来的添加朋友请求，在 新的好友列表 中新增一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendReq, m_contactsPaneWidget.get(), &ContactsPaneWidget::ReceiveSlotAddFriendReq);
    // 接收到好友发来的添加朋友通过请求，在 联系人列表 中新增一项
    connect(this, &MainWindow::SIG_RECEIVE_AddFriendAck, m_contactsPaneWidget.get(), &ContactsPaneWidget::SlotReciveAddFriendAckAgree);
    // 联系人列表中的某一项被选中，则展示对应的内容
    connect(m_contactsPaneWidget.get(), &ContactsPaneWidget::SIG_ItemDidSelected, this, &MainWindow::SlotContactsItemDidSelected);

    auto friends = m_friendModel->FindFriends(m_user->GetUserId());
    for (int i = 0; i < friends.size(); ++i) {
        m_contactsPaneWidget->AddFriendToPane(friends[i]);
    }

}

void MainWindow::InitContactsMainWidget() {
    m_contactsMainWidget = std::make_unique<ContactsMainWidget>();
    connect(m_contactsMainWidget.get(), &ContactsMainWidget::SIG_SEND_AddFriendReqAck, this, &MainWindow::SendSlotAddFriendReqAck);
    // connect(this, &MainWindow::SIG_ReciveAddFriendReq, m_chatPaneWidget, &ChatPaneWidget::SlotReciveAddFriendReq);
}

void MainWindow::SetWidgetWinTitle() {
    ui->winTitleWidget->setFixedHeight(24);

    QSize btnSize(12,12);
    ui->btnMWinClose->setFixedSize(btnSize);
    ui->btnMWinMin->setFixedSize(btnSize);
    ui->btnMWinMax->setFixedSize(btnSize);

    ui->btnMWinClose->setFlat(true);
    ui->btnMWinMin->setFlat(true);
    ui->btnMWinMax->setFlat(true);

    connect(ui->btnMWinClose, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->btnMWinMin, &QPushButton::clicked, this, [=](){
        this->showMinimized();
    });
    connect(ui->btnMWinMax, &QPushButton::clicked, this, [=](){
        this->showMaximized();
    });
}

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
        case Top: case Bottom:          return Qt::SizeVerCursor;
        case Left: case Right:          return Qt::SizeHorCursor;
        case TopLeft: case BottomRight: return Qt::SizeFDiagCursor;
        case TopRight: case BottomLeft: return Qt::SizeBDiagCursor;
        case Center:                    return Qt::SizeAllCursor;
        default:                        return Qt::ArrowCursor;
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
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!m_isMouseDown) return;

    auto currentPos = event->globalPosition().toPoint();
    auto moveSize = currentPos - m_pressPos;
    auto currentRect = m_pressRect;

    setCursor(GetCursorForArea(m_area));

    switch (m_area) {
    case Top:
        currentRect.setTop(m_pressRect.top() + moveSize.y());
        break;
    case Bottom:
        currentRect.setBottom(m_pressRect.bottom() + moveSize.y());
        break;
    case Left:
        currentRect.setLeft(m_pressRect.left() + moveSize.x());
        break;
    case Right:
        currentRect.setRight(m_pressRect.right() + moveSize.x());
        break;
    case TopLeft:
        currentRect.setTopLeft(m_pressRect.topLeft() + moveSize);
        break;
    case TopRight:
        currentRect.setTopRight(m_pressRect.topRight() + moveSize);
        break;
    case BottomLeft:
        currentRect.setBottomLeft(m_pressRect.bottomLeft() + moveSize);
        break;
    case BottomRight:
        currentRect.setBottomRight(m_pressRect.bottomRight() + moveSize);
        break;
    case Center:
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
}

void MainWindow::ShowAddFriendWindow() {
    InitAddFriendWindow();
    m_addFriendWindow->show();
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
    // 取消右侧导航栏之前选中的按钮的选中状态，然后将当前的按钮设置为选中状态
    m_btn->setChecked(false);
    m_btn = (QPushButton*)sender();
    m_btn->setChecked(true);

    switch (m_page) {
    case ChatWidget:
        ui->gridLayout_2->removeWidget(m_chatPaneWidget.get());
        ui->gridLayout_3->removeWidget(m_chatMainWidget.get());
        m_chatPaneWidget->hide();
        m_chatMainWidget->hide();
        break;
    case Contactwidget:
        ui->gridLayout_2->removeWidget(m_contactsPaneWidget.get());
        ui->gridLayout_3->removeWidget(m_contactsMainWidget.get());
        m_contactsPaneWidget->hide();
        m_contactsMainWidget->hide();
        break;
    }

    auto name = m_btn->objectName();
    if (name == "btnAllChat") {
        m_page = ChatPage::ChatWidget;
        ui->gridLayout_2->addWidget(m_chatPaneWidget.get());
        ui->gridLayout_3->addWidget(m_chatMainWidget.get());
        m_chatPaneWidget->show();
        m_chatMainWidget->show();
    } else {
        m_page = ChatPage::Contactwidget;
        ui->gridLayout_2->addWidget(m_contactsPaneWidget.get());
        ui->gridLayout_3->addWidget(m_contactsMainWidget.get());
        m_contactsPaneWidget->show();
        m_contactsMainWidget->show();
    }

}

void MainWindow::SlotChatView(QVariant var, PageType type) {
    if (var.canConvert<Friend*>() && type == PageType::AllChatView) {
        auto data = var.value<Friend*>();
        ui->gridLayout_3->addWidget(m_chatMainWidget.get());
        m_chatMainWidget->SetStackedWidgettCurrentIndex(0);
        m_chatMainWidget->SetData(data);
        m_chatMainWidget->show();
    }
}

void MainWindow::SlotSendChatMsg(int id, const QString& message) {
    qDebug() << "MainWindow::SlotSendChatMsg: userid" << id;
    emit SIG_SendChatMsg(id, message);
}

void MainWindow::SlotContactsItemDidSelected(const std::shared_ptr<ContactsItem>& item) {
    m_contactsMainWidget->SetStackedWidgetCurrentIndex(item);
}

void MainWindow::SendSlotAddFriendReqAck(const User& frienduser, E_ACK_TYPE type) {
    qDebug() << "MainWindow::SlotAddFriendReqAck";
    if (type == E_ACK_TYPE::SUCCESS) {
        // 在联系人列表中添加朋友项
        m_contactsPaneWidget->SlotAddFriendReqAck(frienduser);
        // 在聊天列表中添加朋友项
        m_chatPaneWidget->SlotAddFriendReqAck(frienduser);
    }
    m_friendModel->UpdateFriendState(m_user->GetUserId(), frienduser.GetUserId(), type);

    QJsonObject dataJson;
    dataJson.insert("friendname", QString::fromStdString(frienduser.GetUserName()));
    dataJson.insert("friendid", QString::number(frienduser.GetUserId()));
    dataJson.insert("username", QString::fromStdString(m_user->GetUserName()));
    dataJson.insert("userid", QString::number(m_user->GetUserId()));
    dataJson.insert("ackType", QString::number(static_cast<int>(type)));
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", static_cast<int>(E_MSG_TYPE::ADD_FRIEND_ACK));
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);

    emit SIG_SEND_AddFriendReqAck(data, frienduser);
}

void MainWindow::SlotOneChat(const QByteArray& data) {

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return;
    }

    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();
    qDebug() << "MainWindow::SlotOneChat" << dataObj.value("senderId");
    uint64_t id = dataObj.value("senderId").toString().toLongLong();
    QString content = dataObj.value("content").toString();
    QString createtime = dataObj.value("createtime").toString();
    qDebug() << "MainWindow::SlotOneChat";
    qDebug() << "id: " << id << " content: " << content << " createtime: " << createtime;
    auto item = m_chatPaneWidget->GetItemById(id);
    if (item) {
        Message message(content, createtime, Receive);
        item->UpdateContent(message);
        m_msgModel->AddMsg(m_user->GetUserId(), id, message);
    } else {
        qDebug() << "MainWindow::SlotOneChat failed";
    }
}

void MainWindow::SendSlotAddFriendReq(const User& frienduser) {
    qDebug() << frienduser.GetUserName() << "AppCore SlotAddFriendReq " << frienduser.GetUserId();
    QJsonObject dataJson;
    dataJson.insert("friendname", QString::fromStdString(frienduser.GetUserName()));
    dataJson.insert("friendid", QString::number(frienduser.GetUserId()));
    dataJson.insert("username", QString::fromStdString(m_user->GetUserName()));
    dataJson.insert("userid", QString::number(m_user->GetUserId()));
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", static_cast<int>(E_MSG_TYPE::ADD_FRIEND_REQ));
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    emit SIG_SEND_AddFriendReq(data);
    m_friendModel->AddFriend(m_user->GetUserId(), frienduser, FrinedState::PendingVerification);
}