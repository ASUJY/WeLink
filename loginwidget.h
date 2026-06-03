#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

#include "registerwidget.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void SetWidgetWinTitle();
    void SetWidgetHeadIcon();
    void SetWidgetWinBody();
    void SetWidgetWinBottom();

private:
    Ui::LoginWidget *ui;
    bool m_drag = false;
    QPoint m_dragPos;

    RegisterWidget *m_regWidget;

};

#endif // LOGINWIDGET_H
