#ifndef CONTACTSMAINWIDGET_H
#define CONTACTSMAINWIDGET_H

#include <QWidget>

namespace Ui {
class ContactsMainWidget;
}

class ContactsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsMainWidget(QWidget *parent = nullptr);
    ~ContactsMainWidget();

private:
    Ui::ContactsMainWidget *ui;
};

#endif // CONTACTSMAINWIDGET_H
