#include "addfriendwindow.h"
#include "ui_addfriendwindow.h"
#include <QJsonDocument>
#include <QJsonObject>

AddFriendWindow::AddFriendWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriendWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->btnSearch, &QPushButton::clicked, this, &AddFriendWindow::SlotSearchFriend);
    connect(ui->btnAddFriend, &QPushButton::clicked, this, &AddFriendWindow::SlotAddFriendReq);
}

AddFriendWindow::~AddFriendWindow()
{
    delete ui;
}

void AddFriendWindow::SlotSearchFriend() {
    // 后面可以优化成根据用户名，手机号，账号等进行搜索
    QString username = ui->leditSearch->text();
    if (username.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    }
    emit SIG_GetFriendInfo(username);
}

void AddFriendWindow::SlotGetFriendInfoSuccess(const QByteArray& data) {
    ui->stackedWidget->setCurrentIndex(2);
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError)) return;
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue dataVal = jsonObj.value("data");
    QJsonObject dataObj = dataVal.toObject();

    m_user.SetUserId(dataObj.value("userid").toString().toInt());
    m_user.SetUserName(dataObj.value("username").toString().toStdString());
}


void AddFriendWindow::SlotAddFriendReq() {
    QString username = ui->leditSearch->text();
    if (username.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    }
    emit SIG_AddFriendReq(m_user);
}