#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "loginwidget.h"
#include "registerwidget.h"
#include "mainwindow.h"
#include "communicationmediator.h"

using MsgHandler = std::function<void(const QByteArray& data)>;

class AppCore : public QObject
{
    Q_OBJECT
public:
    AppCore();
    ~AppCore();

public slots:
    void SlotRegisterCommit(QString username, QString phone, QString password);
    void SlotLoginCommit(QString username, QString password);
    void SlotReadyRead(const QByteArray& data, int len);

private:
    MsgHandler GetHandler(int msgtype);
    void RegisterSuccess(const QByteArray& data);
    void LoginSuccess(const QByteArray& data);

private:
    LoginWidget *m_loginWidget = nullptr;
    RegisterWidget* m_registerWidget = nullptr;
    MainWindow* m_mainWindow = nullptr;
    net::CommunicationMediator *m_netMediator = nullptr;
    std::unordered_map<int, std::function<void(const QByteArray& data)>> m_msgHandlerMap;
};

#endif // APPCORE_H
