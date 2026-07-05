#include "appcore.h"
#include "tcpclientmediator.h"
#include "common.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>

AppCore::AppCore() {
    m_loginWidget = new LoginWidget;
    m_registerWidget = new RegisterWidget;
    m_netMediator = new net::TcpClientMediator;
    m_friendModel = std::make_shared<FriendModel>();
    m_msgModel = std::make_shared<MsgModel>();
    m_user = std::make_shared<User>();
    m_mainWindow = new MainWindow(m_user, m_friendModel, m_msgModel);


    // Login
    connect(m_loginWidget, &LoginWidget::SigLoginCommit, this, &AppCore::SlotLoginCommit);

    // Register
    connect(m_loginWidget, &LoginWidget::SigRegister, m_registerWidget, &RegisterWidget::SlotShow);
    connect(m_registerWidget, &RegisterWidget::SigRegisterCommit, this, &AppCore::SlotRegisterCommit);
    connect(m_netMediator, &net::CommunicationMediator::SIG_ReadyData, this, &AppCore::SlotReadyRead);

    // Add Friend
    connect(m_mainWindow, &MainWindow::SIG_GetFriendInfo, this, &AppCore::SlotGetFriendInfo);
    connect(this, &AppCore::SIG_GetFriendInfoSuccess, m_mainWindow, &MainWindow::SlotGetFriendInfoSuccess);
    connect(this, &AppCore::SIG_GetFriendInfoFailed, m_mainWindow, &MainWindow::SlotGetFriendInfoFailed);
    connect(m_mainWindow, &MainWindow::SIG_AddFriendReq, this, &AppCore::SlotAddFriendReq);
    connect(this, &AppCore::SIG_ReciveAddFriendAckAgree, m_mainWindow, &MainWindow::SlotReciveAddFriendAckAgree);

    // Added Friend
    connect(this, &AppCore::SIG_ReciveAddFriendReq, m_mainWindow, &MainWindow::SlotReciveAddFriendReq);
    connect(m_mainWindow, &MainWindow::SIG_AddFriendReqAck, this, &AppCore::SlotAddFriendReqAck);

    // Send Message
    connect(m_mainWindow, &MainWindow::SIG_SendChatMsg, this, &AppCore::SlotSendChatMsg);
    connect(this, &AppCore::SIG_ONECHAT, m_mainWindow, &MainWindow::SlotOneChat);

    m_msgHandlerMap.insert({REG_MSG_ACK_SUCCESS, std::bind(&AppCore::RegisterSuccess, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({LOGIN_MSG_ACK_SUCCESS, std::bind(&AppCore::LoginSuccess, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({GET_FRIEND_INFO_SUCCESS, std::bind(&AppCore::GetFriendInfoSuccess, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({GET_FRIEND_INFO_FAILED, std::bind(&AppCore::GetFriendInfoFailed, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({ADD_FRIEND_REQ, std::bind(&AppCore::SlotReciveAddFriendReq, this, std::placeholders::_1)});
    m_msgHandlerMap.insert({ADD_FRIEND_ACK_AGREE, std::bind(&AppCore::SlotReciveAddFriendAckAgree, this, std::placeholders::_1)});
     m_msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&AppCore::SlotOneChat, this, std::placeholders::_1)});

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

void AppCore::SlotRegisterCommit(QString username, QString phone, QString password) {
    // if (!ConnectServer()) {
    //     qDebug() << "连接服务器失败";
    //     return;
    // }

    QJsonObject fromJson;
    fromJson.insert("userid", -1);
    QJsonObject dataJson;
    dataJson.insert("username", username);
    dataJson.insert("phone", phone);
    dataJson.insert("password", password);
    dataJson.insert("avatar", "");

    QJsonObject json;
    json.insert("from", fromJson);
    json.insert("data", dataJson);
    json.insert("msgtype", REG_MSG);

    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);

    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());
}

void AppCore::SlotLoginCommit(QString username, QString password) {
    // 登录的时候，应该可以用账号，手机号，用户名来进行登录，现在先简单用用户名进行登录，先跑通流程
    QJsonObject dataJson;
    dataJson.insert("username", username);
    dataJson.insert("password", password);

    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", LOGIN_MSG);

    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);

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
    auto it = m_msgHandlerMap.find(msgtype);
    if (it == m_msgHandlerMap.end()) {
        // 返回一个默认的处理器，空操作
        return [=](const QByteArray& data){
            qDebug() << "msgtype:" << msgtype << " can not find handler!";
        };
    }
    return it->second;
}

void AppCore::RegisterSuccess(const QByteArray& data) {
    // 临时弹窗告诉用户注册成功，后面有时间再优化UI界面
    QMessageBox::about(this->m_registerWidget, "提示", "注册成功");
}

void AppCore::LoginSuccess(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        return;
    }

    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    m_user->SetUserId(dataObj.value("userid").toInt());
    m_user->SetUserName(dataObj.value("username").toString().toStdString());
    m_user->SetUserPhone(dataObj.value("phone").toString().toStdString());
    m_user->SetUserAvatar(dataObj.value("avator").toString().toStdString());
    if (!m_userModel.IsUserExit(m_user)) {
        m_userModel.AddUser(m_user);
    }

    m_mainWindow->Init();
    m_loginWidget->hide();
    m_mainWindow->show();


    // if (dataObj.contains("offlinemsg")) {
    //     QJsonArray array = dataObj["offlinemsg"].toArray();
    //     for (int i = 0; i < array.size(); ++i) {
    //         QJsonValue val = array[i];
    //         if (val.isObject()) {
    //             emit SIG_ONECHAT(val.toJson());
    //         }
    //     }
    // }
}

void AppCore::SlotGetFriendInfo(const QString& username) {
    QJsonObject dataJson;
    dataJson.insert("username", username);
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", GET_FRIEND_INFO_REQ);
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);
    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());

}

void AppCore::GetFriendInfoSuccess(const QByteArray& data) {
    emit SIG_GetFriendInfoSuccess(data);
}

void  AppCore::GetFriendInfoFailed(const QByteArray& data) {
    emit SIG_GetFriendInfoFailed(data);
}

void  AppCore::SlotAddFriendReq(const User& frienduser) {
    qDebug() << frienduser.GetUserName() << "AppCore SlotAddFriendReq " << frienduser.GetUserId();
    QJsonObject dataJson;
    dataJson.insert("friendname", QString::fromStdString(frienduser.GetUserName()));
    dataJson.insert("friendid", QString::number(frienduser.GetUserId()));
    dataJson.insert("username", QString::fromStdString(m_user->GetUserName()));
    dataJson.insert("userid", QString::number(m_user->GetUserId()));
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", ADD_FRIEND_REQ);
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);
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
    json.insert("msgtype", ADD_FRIEND_ACK_AGREE);
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
    json.insert("msgtype", ONE_CHAT_MSG);
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
