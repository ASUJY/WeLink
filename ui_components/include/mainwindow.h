#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "chatpanewidget.h"
#include "contactspanewidget.h"
#include "addfriendwindow.h"

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
    void SIG_GetFriendInfo(QString username);
    void SIG_GetFriendInfoSuccess(const QByteArray& data);
    void SIG_AddFriendReq(User user);

public slots:
    void SlotGetFriendInfoSuccess(const QByteArray& data);

private slots:
    void SlotSelectEvent();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void SetWidgetWinTitle();
    Area GetArea(int x, int y);
    Qt::CursorShape GetCursorForArea(Area area);
    void ShowAddFriendWindow();

private:
    Ui::MainWindow *ui;

    // 当前选中的按钮(右侧导航栏的按钮)
    QPushButton *m_btn;
    ChatPage m_page;

    bool m_isMouseDown = false;
    QPoint m_pressPos;
    QRect m_pressRect;
    int m_posx;
    int m_posy;
    Area m_area;

    ChatPaneWidget *m_chatPaneWidget;
    ContactsPaneWidget *m_contactsPaneWidget;
    AddFriendWindow *m_addFriendWindow;

};
#endif // MAINWINDOW_H
