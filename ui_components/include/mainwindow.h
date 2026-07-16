#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "chatpanewidget.h"
#include "chatmainwidget.h"
#include "contactspanewidget.h"
#include "contactsmainwidget.h"
#include "addfriendwindow.h"
#include "friendmodel.h"
#include "msgmodel.h"

enum Area {
    Top,
    Left,
    Right,
    Bottom,
    Center,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

enum ChatPage {
    ChatWidget = 0,
    Contactwidget
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void SIG_SEND_GetFriendInfo(const QByteArray& data);
    void SIG_GetFriendInfoSuccess(const QByteArray& data);
    void SIG_GetFriendInfoFailed(const QByteArray& data);
    void SIG_AddFriendReq(const User& user);
    void SIG_AddFriendReqAck(const User& user);
    void SIG_ReciveAddFriendReq(const QByteArray& data);
    void SIG_ReciveAddFriendAckAgree(const QByteArray& data);
    void SIG_SendChatMsg(int id, const QString &message);

public slots:
    void SlotGetFriendInfoSuccess(const QByteArray& data);
    void SlotGetFriendInfoFailed(const QByteArray& data);
    void SlotReciveAddFriendReq(const QByteArray& data);
    void SlotAddFriendReqAck(const User& user);
    void SlotReciveAddFriendAckAgree(const QByteArray& data);
    void SlotSendChatMsg(int id, const QString &message);
    void SlotContactsItemDidSelected(const std::shared_ptr<ContactsItem>&);
    void SlotOneChat(const QByteArray& data);

private slots:
    void SlotSelectEvent();
    void SlotChatView(QVariant var, PageType type);

public:
    explicit MainWindow(std::shared_ptr<User> user, std::shared_ptr<FriendModel> friendModel, std::shared_ptr<MsgModel> msgModel, QWidget *parent = nullptr);
    ~MainWindow() override;
    void Init();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    void InitAddFriendWindow();
    void InitChatMainWidget();
    void InitChatPaneWidget();
    void InitContactsPaneWidget();
    void InitContactsMainWidget();
    void SetWidgetWinTitle();
    Area GetArea(int x, int y);
    Qt::CursorShape GetCursorForArea(Area area);
    void ShowAddFriendWindow();

private:
    Ui::MainWindow *ui;

    // 当前选中的按钮(右侧导航栏的按钮)
    QPushButton* m_btn;
    ChatPage m_page = ChatWidget;

    bool m_isMouseDown = false;
    QPoint m_pressPos;
    QRect m_pressRect;
    int m_posx = 0;
    int m_posy = 0;
    Area m_area;

    std::unique_ptr<ChatPaneWidget> m_chatPaneWidget;
    std::unique_ptr<ChatMainWidget> m_chatMainWidget;
    std::unique_ptr<ContactsPaneWidget> m_contactsPaneWidget;
    std::unique_ptr<ContactsMainWidget> m_contactsMainWidget;
    AddFriendWindow* m_addFriendWindow;

    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<MsgModel> m_msgModel;
    std::shared_ptr<User> m_user;

};
#endif // MAINWINDOW_H
