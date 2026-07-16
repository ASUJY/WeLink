#ifndef ADDFRIENDWINDOW_H
#define ADDFRIENDWINDOW_H

#include <QDialog>
#include "user.hpp"

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
    void SIG_AddFriendReq(const User& user);

public slots:
    void SlotGetFriendInfoSuccess(const QByteArray& data);
    void SlotGetFriendInfoFailed(const QByteArray& data);
    void SlotChangedStackWidget();

private slots:
    void SlotSearchFriend();
    void SlotAddFriendReq();
    // void SlotGetFriendInfo();

private:
    Ui::AddFriendWindow *ui;
    User m_user;
};

#endif // ADDFRIENDWINDOW_H
