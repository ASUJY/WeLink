#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include "common.h"

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

signals:
    void SIG_Register();
    void SIG_LoginCommit(const QByteArray& data);

private slots:
    void OnBtnLoginClicked();

private:
    void SetWidgetWinTitle();
    void SetWidgetHeadIcon();
    void SetWidgetWinBody();
    void SetWidgetWinBottom();
    QByteArray MakeLoginJSON();
    E_ACCOUNT_TYPE CheckAccountType();

private:
    Ui::LoginWidget *ui;
    bool m_drag = false;
    QPoint m_dragPos;

};

#endif // LOGINWIDGET_H
