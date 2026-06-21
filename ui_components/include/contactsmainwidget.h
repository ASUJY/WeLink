#ifndef CONTACTSMAINWIDGET_H
#define CONTACTSMAINWIDGET_H

#include <QWidget>
#include "contactslistviewchild.h"

namespace Ui {
class ContactsMainWidget;
}

class ContactsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsMainWidget(QWidget *parent = nullptr);
    ~ContactsMainWidget();
    void SetStackedWidgetCurrentIndex(ContactsListViewChild*);

private:
    Ui::ContactsMainWidget *ui;
};

#endif // CONTACTSMAINWIDGET_H
