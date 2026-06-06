#include "appcore.h"
#include "net/tcpclientmediator.h"
#include "common.h"

#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>

AppCore::AppCore() {
    m_loginWidget = new LoginWidget;
    m_registerWidget = new RegisterWidget;
    m_netMediator = new net::TcpClientMediator;

    connect(m_loginWidget, &LoginWidget::SigRegister, m_registerWidget, &RegisterWidget::SlotShow);
    connect(m_registerWidget, &RegisterWidget::SigRegisterCommit, this, &AppCore::SlotRegisterCommit);
    connect(m_netMediator, &net::CommunicationMediator::SIG_ReadyData, this, &AppCore::SlotReadyRead);

    m_msgHandlerMap.insert({REG_MSG_ACK_SUCCESS, std::bind(&AppCore::RegisterSuccess, this, std::placeholders::_1)});

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
    json.insert("msgtype", REG_MSG);

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
        m_msgHandlerMap[msgtype](data);
    }
}

void AppCore::RegisterSuccess(const QByteArray& data) {
    // 临时弹窗告诉用户注册成功，后面有时间再优化UI界面
    QMessageBox::about(this->m_registerWidget, "提示", "注册成功");
}
