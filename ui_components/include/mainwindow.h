#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "chatlistwidget.h"

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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

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

private:
    Ui::MainWindow *ui;

    bool m_isMouseDown = false;
    QPoint m_pressPos;
    QRect m_pressRect;
    int m_posx;
    int m_posy;
    Area m_area;

    ChatListWidget *m_allChatListWidget;
};
#endif // MAINWINDOW_H
