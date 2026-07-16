#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "loginwidget.h"
#include "registerwidget.h"
#include "mainwindow.h"
#include "communicationmediator.h"
#include "user.hpp"
#include "usermodel.h"
#include "friendmodel.h"
#include "msgmodel.h"
#include "common.h"

using MsgHandler = std::function<void(const QByteArray& data)>;

class AppCore : public QObject
{
    Q_OBJECT

signals:
    void SIG_GetFriendInfoSuccess(const QByteArray& data);
    void SIG_GetFriendInfoFailed(const QByteArray& data);
    void SIG_ReciveAddFriendReq(const QByteArray& data);
    void SIG_ReciveAddFriendAckAgree(const QByteArray& data);
    void SIG_ONECHAT(const QByteArray& data);

public:
    AppCore();
    ~AppCore();

public slots:
    void SlotRegisterCommit(const QByteArray& data);
    void SlotLoginCommit(const QByteArray& data);
    void SlotReadyRead(const QByteArray& data, int len);
    void SlotGetFriendInfo(const QString& username);
    void SlotAddFriendReq(const User& user);
    void SlotReciveAddFriendReq(const QByteArray& data);
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReqAck(const User& user);
    void SlotSendChatMsg(int id, const QString& content);
    void SlotOneChat(const QByteArray& data);

private slots:
    void SlotShowRegisterWidget();

private:
    MsgHandler GetHandler(int msgtype);
    void RegisterSuccess(const QByteArray& data);
    void ReceiveLoginACK(const QByteArray& data);
    void GetFriendInfoSuccess(const QByteArray& data);
    void GetFriendInfoFailed(const QByteArray& data);

private:
    LoginWidget *m_loginWidget = nullptr;
    RegisterWidget* m_registerWidget = nullptr;
    std::unique_ptr<MainWindow> m_mainWindow = nullptr;
    std::unique_ptr<net::CommunicationMediator> m_netMediator = nullptr;
    std::unordered_map<E_MSG_TYPE, std::function<void(const QByteArray& data)>> m_msgHandlerMap;
    std::shared_ptr<User> m_user;
    UserModel m_userModel;
    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<MsgModel> m_msgModel;
};

#endif // APPCORE_H
