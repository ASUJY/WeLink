#ifndef ADDFRIENDWINDOW_H
#define ADDFRIENDWINDOW_H

#include <QDialog>

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
    void SIG_GetFriendInfo(QString username);

public slots:
    void SlotGetFriendInfoSuccess(const QByteArray& data);

private slots:
    void SlotSearchFriend();
    // void SlotGetFriendInfo();

private:
    Ui::AddFriendWindow *ui;
};

#endif // ADDFRIENDWINDOW_H
