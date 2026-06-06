#include "appcore.h"
#include "net/tcpclientmediator.h"
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>

AppCore::AppCore() {
    m_loginWidget = new LoginWidget;
    m_registerWidget = new RegisterWidget;
    m_netMediator = new net::TcpClientMediator;

    connect(m_loginWidget, &LoginWidget::SigRegister, m_registerWidget, &RegisterWidget::SlotShow);
    connect(m_registerWidget, &RegisterWidget::SigRegisterCommit, this, &AppCore::SlotRegisterCommit);
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

    QJsonObject json;
    json.insert("from", fromJson);
    json.insert("data", dataJson);
    json.insert("msgtype", 1);

    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);
    qDebug() << "m_tcpSocket->write:" << document.toJson(QJsonDocument::Compact);

    // 后面可以根据返回值来判断数据是否发送成功，或者根据返回的状态码告诉用户是网络问题还是密码错误之类的原因
    bool res = m_netMediator->SendData(data, data.size());
}