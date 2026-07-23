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
#include "friendrequestmodel.h"

using MsgHandler = std::function<void(const QByteArray& data)>;

class AppCore : public QObject
{
    Q_OBJECT

signals:
    void SIG_RECEIVE_GetFriendInfoACK(const QByteArray& data);
    void SIG_RECEIVE_GetFriendInfoACK(const User& user);
    void SIG_RECEIVE_AddFriendReq(const QByteArray& data);
    void SIG_RECEIVE_AddFriendAck(const QByteArray& data);
    void SIG_ONECHAT(const QByteArray& data);

public:
    AppCore() noexcept;
    ~AppCore();

public slots:
    void SlotRegisterCommit(const QByteArray& data);
    void SlotLoginCommit(const QByteArray& data);
    void SlotReadyRead(const QByteArray& data, int len);
    void SendSlotGetFriendInfo(const QByteArray& data, const QString& name, E_ACCOUNT_TYPE type);
    void SendSlotAddFriendReq(const QByteArray& data);
    void ReceiveSlotAddFriendReq(const QByteArray& data);
    void ReceiveSlotAddFriendAck(const QByteArray& data);
    void SendSlotAddFriendReqAck(const QByteArray& data, const User& user, FriendState type);
    void SlotSendChatMsg(int64_t id, const QString& content);
    void SlotOneChat(const QByteArray& data);

private slots:
    void SlotShowRegisterWidget();

private:
    MsgHandler GetHandler(int msgtype);
    void RegisterSuccess(const QByteArray& data);
    void ReceiveLoginACK(const QByteArray& data);
    void ReceiveGetFriendInfoACK(const QByteArray& data);

    QByteArray BuildOneChatPacket(int64_t receiverId, const QString& content, const QString& currentTime);

private:
    LoginWidget *m_loginWidget = nullptr;
    RegisterWidget* m_registerWidget = nullptr;
    std::unique_ptr<MainWindow> m_mainWindow = nullptr;
    std::unique_ptr<net::CommunicationMediator> m_netMediator = nullptr;
    std::unordered_map<E_MSG_TYPE, MsgHandler> m_msgHandlerMap;
    std::shared_ptr<User> m_user;
    UserModel m_userModel;
    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<MsgModel> m_msgModel;
    std::shared_ptr<FriendRequestModel> m_friendRequestModel;
};

#endif // APPCORE_H
