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
signals:
    void SIG_SEND_GetFriendInfo(const QByteArray& data, const QString& name, E_ACCOUNT_TYPE type);
    void SIG_SEND_AddFriendReq(const User& user);

public slots:
    void ReceiveSlotGetFriendInfoACK(const QByteArray& data);
    void ReceiveSlotGetFriendInfoACK(const User& user);
    void SlotChangedStackWidget();

private slots:
    void SlotSearchFriend();
    void SlotAddFriendReq();

public:
    explicit AddFriendWindow(QWidget *parent = nullptr);
    ~AddFriendWindow() override;

private:
    E_ACCOUNT_TYPE CheckAccountType() const;

private:
    Ui::AddFriendWindow *ui;
    User m_user;
};

#endif // ADDFRIENDWINDOW_H
