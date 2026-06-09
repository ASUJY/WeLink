#include "addfriendwindow.h"
#include "ui_addfriendwindow.h"

AddFriendWindow::AddFriendWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriendWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->btnSearch, &QPushButton::clicked, this, &AddFriendWindow::SlotSearchFriend);
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
}
