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
#include "friendrequestmodel.h"

enum class Area : int {
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

enum class ChatPage : int {
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
    void SIG_SEND_GetFriendInfo(const QByteArray& data, const QString& name, E_ACCOUNT_TYPE type);
    void SIG_RECEIVE_GetFriendInfoACK(const QByteArray& data);
    void SIG_RECEIVE_GetFriendInfoACK(const User& user);
    // void SIG_GetFriendInfoFailed(const QByteArray& data);
    void SIG_SEND_AddFriendReq(const QByteArray& data);
    void SIG_SEND_AddFriendReqAck(const QByteArray& data, const User& user, FriendState type);
    void SIG_RECEIVE_AddFriendReq(const QByteArray& data);
    void SIG_RECEIVE_AddFriendAck(const QByteArray& data);
    void SIG_SendChatMsg(int64_t id, const QString &message);

public slots:
    void ReceiveSlotGetFriendInfoACK(const QByteArray& data);
    void SlotGetFriendInfoFailed(const QByteArray& data);
    void ReceiveSlotAddFriendReq(const QByteArray& data);
    void SendSlotAddFriendReqAck(const User& user, FriendState type);
    void ReceiveSlotAddFriendAck(const QByteArray& data);
    void SlotSendChatMsg(int64_t id, const QString &message);
    void SlotContactsItemDidSelected(const std::shared_ptr<ContactsItem>&);
    void SlotOneChat(const QByteArray& data);
    void SendSlotAddFriendReq(const User& frienduser);

private slots:
    void SlotSelectEvent();
    void SlotChatView(QVariant var, PageType type);

public:
    explicit MainWindow(std::shared_ptr<User> user, std::shared_ptr<FriendModel> friendModel,
                        std::shared_ptr<MsgModel> msgModel, std::shared_ptr<FriendRequestModel> friendRequestModel,
                        QWidget *parent = nullptr) noexcept;
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

    QByteArray BuildAddFriendRequestPacket(const User& self, const User& target);
    QByteArray BuildAddFriendAckPacket(const User& self, const User& target, FriendState state);

private:
    Ui::MainWindow *ui;

    // 当前选中的按钮(右侧导航栏的按钮)
    QPushButton* m_btn;
    ChatPage m_page = ChatPage::ChatWidget;

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
    AddFriendWindow* m_addFriendWindow = nullptr;

    std::shared_ptr<FriendModel> m_friendModel;
    std::shared_ptr<FriendRequestModel> m_friendRequestModel;
    std::shared_ptr<MsgModel> m_msgModel;
    std::shared_ptr<User> m_user;

};
#endif // MAINWINDOW_H
