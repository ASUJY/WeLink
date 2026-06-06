#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "loginwidget.h"
#include "registerwidget.h"
#include "net/communicationmediator.h"

class AppCore : public QObject
{
    Q_OBJECT
public:
    AppCore();
    ~AppCore();

public:
    void SlotRegisterCommit(QString username, QString phone, QString password);

private:
    LoginWidget *m_loginWidget = nullptr;
    RegisterWidget* m_registerWidget = nullptr;
    net::CommunicationMediator *m_netMediator = nullptr;
};

#endif // APPCORE_H
