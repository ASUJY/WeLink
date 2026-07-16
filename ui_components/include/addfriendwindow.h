#ifndef ADDFRIENDWINDOW_H
#define ADDFRIENDWINDOW_H

#include <QDialog>
#include "user.hpp"
#include "common.h"

namespace Ui {
class AddFriendWindow;
}

class AddFriendWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendWindow(QWidget *parent = nullptr);
    ~AddFriendWindow();

signals:
    void SIG_SEND_GetFriendInfo(const QByteArray& data);
    void SIG_SEND_AddFriendReq(const User& user);

public slots:
    void ReceiveSlotGetFriendInfoACK(const QByteArray& data);
    void SlotChangedStackWidget();

private slots:
    void SlotSearchFriend();
    void SlotAddFriendReq();
    // void SlotGetFriendInfo();

private:
    E_ACCOUNT_TYPE CheckAccountType();

private:
    Ui::AddFriendWindow *ui;
    User m_user;
};

#endif // ADDFRIENDWINDOW_H
