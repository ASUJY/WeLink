#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private:
    void SetWidgetWinTitle();
    void SetWidgetHeadIcon();
    void SetWidgetWinBody();
    void SetWidgetWinBottom();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
