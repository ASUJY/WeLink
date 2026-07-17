#include "addfriendwindow.h"
#include "ui_addfriendwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>
#include "user.hpp"
#include "common.h"

AddFriendWindow::AddFriendWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriendWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->btnSearch, &QPushButton::clicked, this, &AddFriendWindow::SlotSearchFriend);
    connect(ui->btnAddFriend, &QPushButton::clicked, this, &AddFriendWindow::SlotAddFriendReq);
    connect(ui->leditSearch, &QLineEdit::textChanged, this, &AddFriendWindow::SlotChangedStackWidget);
}

AddFriendWindow::~AddFriendWindow()
{
    delete ui;
}

E_ACCOUNT_TYPE AddFriendWindow::CheckAccountType() {
    // 去除首尾空格
    QString text = ui->leditSearch->text().trimmed();

    // 手机号正则表达式（大陆 11 位，1 开头）
    QRegularExpression phoneReg("^1[3-9]\\d{9}$");
    QRegularExpressionMatch match = phoneReg.match(text);

    // 手机号
    if (match.hasMatch()) {
        return E_ACCOUNT_TYPE::PHONE;
    }

    // 用户名
    return E_ACCOUNT_TYPE::NICKNAME;
}

void AddFriendWindow::SlotSearchFriend() {
    // 后面可以优化成根据用户名，手机号，账号等进行搜索
    QString username = ui->leditSearch->text();
    if (username.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    }
    E_ACCOUNT_TYPE accountType = CheckAccountType();
    QJsonObject dataJson;
    dataJson.insert("username", username);
    dataJson.insert("accountType", QString::number(static_cast<int>(accountType)));
    QJsonObject json;
    json.insert("data", dataJson);
    json.insert("msgtype", static_cast<int>(E_MSG_TYPE::GET_FRIEND_INFO_REQ));
    QJsonDocument document;
    document.setObject(json);

    auto data = document.toJson(QJsonDocument::Compact);

    emit SIG_SEND_GetFriendInfo(data, username, accountType);
}

void AddFriendWindow::ReceiveSlotGetFriendInfoACK(const QByteArray& data) {
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    E_ERR_TYPE errtype = static_cast<E_ERR_TYPE>(dataObj.value("errtype").toInt());
    if (errtype == E_ERR_TYPE::GET_FRIEND_INFO_FAILED) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    } else if (errtype == E_ERR_TYPE::GET_FRIEND_INFO_SUCCESS) {
        ui->stackedWidget->setCurrentIndex(2);
        m_user.SetUserId(dataObj.value("userid").toString().toInt());
        m_user.SetUserName(dataObj.value("username").toString().toStdString());
        ui->labHeadIcon->setPixmap(QPixmap(":/resource/head/man.svg"));
        ui->labName->setText(QString::fromStdString(m_user.GetUserName()));
    }
}

void AddFriendWindow::ReceiveSlotGetFriendInfoACK(const User& user) {
    ui->stackedWidget->setCurrentIndex(3);
    ui->labHeadIcon_2->setPixmap(QPixmap(":/resource/head/man.svg"));
    ui->labName_2->setText(QString::fromStdString(user.GetUserName()));
    return;
}

void AddFriendWindow::SlotChangedStackWidget() {
    ui->stackedWidget->setCurrentIndex(0);
}


void AddFriendWindow::SlotAddFriendReq() {
    QString username = ui->leditSearch->text();
    if (username.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    }
    emit SIG_SEND_AddFriendReq(m_user);
    this->close();
    // this->hide();
    // this->close();
}