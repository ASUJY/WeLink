#include "appcore.h"
#include "tcpclientmediator.h"
#include "common.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>

AppCore::AppCore() {
    m_loginWidget = new LoginWidget;
    m_loginWidget->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_loginWidget, &LoginWidget::destroyed, this, [this](){
        m_loginWidget = nullptr;
    });

    m_netMediator = std::make_unique<net::TcpClientMediator>();
    m_friendModel = std::make_shared<FriendModel>();
    m_msgModel = std::make_shared<MsgModel>();
    m_user = std::make_shared<User>();
    m_mainWindow = std::make_unique<MainWindow>(m_user, m_friendModel, m_msgModel);

    // Login
    connect(m_loginWidget, &LoginWidget::SIG_LoginCommit, this, &AppCore::SlotLoginCommit);

    // Register
    connect(m_loginWidget, &LoginWidget::SIG_Register, this, &AppCore::SlotShowRegisterWidget);

    connect(m_netMediator.get(), &net::CommunicationMediator::SIG_ReadyData, this, &AppCore::SlotReadyRead);

    // Add Friend
    connect(m_mainWindow.get(), &MainWindow::SIG_SEND_GetFriendInfo, this, &AppCore::SendSlotGetFriendInfo);
    connect(this, &AppCore::SIG_RECEIVE_GetFriendInfoACK, m_mainWindow.get(), &MainWindow::ReceiveSlotGetFriendInfoACK);
    // connect(this, &::AppCore::GetFriendInfoFailed, m_mainWindow.get(), &MainWindow::SlotGetFriendInfoFailed);
    connect(m_mainWindow.get(), &MainWindow::SIG_SEND_AddFriendReq, this, &AppCore::SendSlotAddFriendReq);
    connect(this, &AppCore::SIG_ReciveAddFriendAckAgree, m_mainWindow.get(), &MainWindow::SlotReciveAddFriendAckAgree);

    // Added Friend
    connect(this, &AppCore::SIG_ReciveAddFriendReq, m_mainWindow.get(), &MainWindow::SlotReciveAddFriendReq);
    connect(m_mainWindow.get(), &MainWindow::SIG_AddFriendReqAck, this, &AppCore::SlotAddFriendReqAck);

    // Send Message
    connect(m_mainWindow.get(), &MainWindow::SIG_SendChatMsg, this, &AppCore::SlotSendChatMsg);
    connect(this, &AppCore::SIG_ONECHAT, m_mainWindow.get(), &MainWindow::SlotOneChat);

    m_msgHandlerMap.insert({E_MSG_TYPE::REG_MSG_ACK, std::bind(&AppCore::RegisterSuccess, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({E_MSG_TYPE::LOGIN_MSG_ACK, std::bind(&AppCore::ReceiveLoginACK, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({E_MSG_TYPE::GET_FRIEND_INFO_ACK, std::bind(&AppCore::ReceiveGetFriendInfoACK, this, std::placeholders::_1)});
    // m_msgHandlerMap.insert({E_MSG_TYPE::GET_FRIEND_INFO_FAILED, std::bind(&AppCore::GetFriendInfoFailed, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({E_MSG_TYPE::ADD_FRIEND_REQ, std::bind(&AppCore::SlotReciveAddFriendReq, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({E_MSG_TYPE::ADD_FRIEND_ACK_AGREE, std::bind(&AppCore::SlotReciveAddFriendAckAgree, this, std::placeholders::_1)});
     m_msgHandlerMap.insert({E_MSG_TYPE::ONE_CHAT_MSG, std::bind(&AppCore::SlotOneChat, this, std::placeholders::_1)});

    m_loginWidget->show();
}

AppCore::~AppCore() {
    if (m_loginWidget) {
        m_loginWidget->hide();
        delete m_loginWidget;
        m_loginWidget = nullptr;
    }
    if (m_registerWidget) {
        m_registerWidget->hide();
        delete m_registerWidget;
        m_registerWidget = nullptr;
    }
}

void AppCore::SlotShowRegisterWidget() {
    if (m_registerWidget == nullptr) {
        m_registerWidget = new RegisterWidget;
        m_registerWidget->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_registerWidget, &RegisterWidget::SIG_RegisterCommit, this, &AppCore::SlotRegisterCommit);
        connect(m_registerWidget, &RegisterWidget::destroyed, this, [this](){
            m_registerWidget = nullptr;
        });
        m_registerWidget->show();
    }
}

void AppCore::SlotRegisterCommit(const QByteArray& data) {
    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());
}

void AppCore::SlotLoginCommit(const QByteArray& data) {
    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());

}

void AppCore::SlotReadyRead(const QByteArray& data, int len) {
    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDocument.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return;
    }
    if (jsonDocument.isObject()) {
        QJsonObject jsonObj = jsonDocument.object();
        int msgtype = jsonObj.value("msgtype").toInt();
        auto msgHandler = GetHandler(msgtype);
        msgHandler(data);

    }
}

MsgHandler  AppCore::GetHandler(int msgtype) {
    auto it = m_msgHandlerMap.find(static_cast<E_MSG_TYPE>(msgtype));
    if (it == m_msgHandlerMap.end()) {
        // 返回一个默认的处理器，空操作
        return [=](const QByteArray& data){
            qDebug() << "msgtype:" << msgtype << " can not find handler!";
        };
    }
    return it->second;
}

void AppCore::RegisterSuccess(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return;
    }
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    E_ERR_TYPE errtype = static_cast<E_ERR_TYPE>(dataObj.value("errtype").toInt());
    if (errtype == E_ERR_TYPE::REG_MSG_ACK_SUCCESS) {
        if (this->m_registerWidget == nullptr) {
            return;
        }
        // 临时弹窗告诉用户注册成功，后面有时间再优化UI界面
        QMessageBox::about(this->m_registerWidget, "提示", "注册成功");
    } else if (errtype == E_ERR_TYPE::USER_EXIT){
        if (this->m_registerWidget == nullptr) {
            return;
        }
        QMessageBox::about(this->m_registerWidget, "提示", "用户已存在");
    }

}

void AppCore::ReceiveLoginACK(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return;
    }

    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    E_ERR_TYPE errtype = static_cast<E_ERR_TYPE>(dataObj.value("errtype").toInt());
    if (errtype == E_ERR_TYPE::ACCOUNT_ERROR) {
        QMessageBox::about(this->m_loginWidget, "提示", "用户名密码错误");
        return;
    } else if (errtype == E_ERR_TYPE::USER_NO_REGISTER) {
        QMessageBox::about(this->m_loginWidget, "提示", "用户未注册");
        return;
    } else if (errtype == E_ERR_TYPE::USER_ONLINE) {
        QMessageBox::about(this->m_loginWidget, "提示", "用户已登录");
        return;
    } else if (errtype == E_ERR_TYPE::LOGIN_MSG_ACK_SUCCESS) {

        m_user->SetUserId(dataObj.value("userid").toInt());
        m_user->SetUserName(dataObj.value("username").toString().toStdString());
        m_user->SetUserPhone(dataObj.value("phone").toString().toStdString());
        m_user->SetUserAvatar(dataObj.value("avator").toString().toStdString());
        if (!m_userModel.IsUserExit(m_user)) {
            m_userModel.AddUser(m_user);
        }

        m_mainWindow->Init();

        if (jsonObj.contains("offlinemsg")) {
            QJsonArray array = jsonObj["offlinemsg"].toArray();
            for (int i = 0; i < array.size(); ++i) {
                QString jsonStr = array[i].toString();
                QJsonDocument jsonDoc1 = QJsonDocument::fromJson(jsonStr.toUtf8());
                emit SIG_ONECHAT(jsonDoc1.toJson());
            }
        }

        m_loginWidget->hide();
        m_mainWindow->show();
        m_loginWidget->close();
    }
}

void AppCore::SendSlotGetFriendInfo(const QByteArray& data) {

    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());

}

void AppCore::ReceiveGetFriendInfoACK(const QByteArray& data) {
    emit SIG_RECEIVE_GetFriendInfoACK(data);
}

void  AppCore::SendSlotAddFriendReq(const QByteArray& data) {

    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());
}

void AppCore::SlotReciveAddFriendReq(const QByteArray& data) {
    qDebug() << "申请添加朋友";
    emit SIG_ReciveAddFriendReq(data);
}

void AppCore::SlotReciveAddFriendAckAgree(const QByteArray& data) {
    qDebug() << "对方已同意添加为朋友";
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    // if (jsontmp.isNull() || !jsontmp.isObject()) return;

    QJsonObject dataObj = dataVal.toObject();
    QString friendname = dataObj.value("sendername").toString();
    int friendid = dataObj.value("senderid").toInt();
    Friend fri;
    fri.SetUserId(friendid);
    fri.SetUserName(friendname.toStdString());

    if (!m_friendModel->IsFriendExit(m_user->GetUserId(), fri)) {
        m_friendModel->AddFriend(m_user->GetUserId(), fri);
        emit SIG_ReciveAddFriendAckAgree(data);
    }
}

void AppCore::SlotAddFriendReqAck(const User& frienduser) {
    qDebug() << frienduser.GetUserName() << "AppCore SlotAddFriendReqAck " << frienduser.GetUserId();
    QJsonObject dataJson;
    dataJson.insert("friendname", QString::fromStdString(frienduser.GetUserName()));
    dataJson.insert("friendid", QString::number(frienduser.GetUserId()));
    dataJson.insert("username", QString::fromStdString(m_user->GetUserName()));
    dataJson.insert("userid", QString::number(m_user->GetUserId()));
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", static_cast<int>(E_MSG_TYPE::ADD_FRIEND_ACK_AGREE));
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);

    Friend fri;
    fri.SetUserId(frienduser.GetUserId());
    fri.SetUserName(frienduser.GetUserName());
    if (!m_friendModel->IsFriendExit(m_user->GetUserId(), fri)) {
        m_friendModel->AddFriend(m_user->GetUserId(), fri);
        // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
        bool res = m_netMediator->SendData(data, data.size());
    }
}

void AppCore::SlotSendChatMsg(int id, const QString& content) {
    qDebug() << "AppCore::SlotSendChatMsg userid: " << id;
    QJsonObject dataJson;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    dataJson.insert("senderId", QString::number(m_user->GetUserId()));
    dataJson.insert("receiverId", QString::number(id));
    dataJson.insert("content", content);
    dataJson.insert("createtime", currentTime);
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", static_cast<int>(E_MSG_TYPE::ONE_CHAT_MSG));
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);
    bool res = m_netMediator->SendData(data, data.size());

    Message msg(content, currentTime, MsgType::Sender);
    m_msgModel->AddMsg(m_user->GetUserId(), id, msg);
}

void AppCore::SlotOneChat(const QByteArray& data) {
    qDebug() << "AppCore::SlotOneChat";
    emit SIG_ONECHAT(data);
}
