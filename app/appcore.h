#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "loginwidget.h"
#include "registerwidget.h"
#include "mainwindow.h"
#include "communicationmediator.h"
#include "user.hpp"
#include "usermodel.h"

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
    void SlotRegisterCommit(QString username, QString phone, QString password);
    void SlotLoginCommit(QString username, QString password);
    void SlotReadyRead(const QByteArray& data, int len);
    void SlotGetFriendInfo(QString username);
    void SlotAddFriendReq(User user);
    void SlotReciveAddFriendReq(const QByteArray& data);
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotAddFriendReqAck(User user);
    void SlotSendChatMsg(int id, const QString& content);
    void SlotOneChat(const QByteArray& data);


private:
    MsgHandler GetHandler(int msgtype);
    void RegisterSuccess(const QByteArray& data);
    void LoginSuccess(const QByteArray& data);
    void GetFriendInfoSuccess(const QByteArray& data);
    void GetFriendInfoFailed(const QByteArray& data);

private:
    LoginWidget *m_loginWidget = nullptr;
    RegisterWidget* m_registerWidget = nullptr;
    MainWindow* m_mainWindow = nullptr;
    net::CommunicationMediator *m_netMediator = nullptr;
    std::unordered_map<int, std::function<void(const QByteArray& data)>> m_msgHandlerMap;
    User m_user;
    UserModel m_userModel;
};

#endif // APPCORE_H
