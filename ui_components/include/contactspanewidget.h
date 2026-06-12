#ifndef CONTACTSPANEWIDGET_H
#define CONTACTSPANEWIDGET_H

#include <QWidget>

namespace Ui {
class ContactsPaneWidget;
}

class ContactsPaneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsPaneWidget(QWidget *parent = nullptr);
    ~ContactsPaneWidget();

public slots:
    void SlotReciveAddFriendReq(const QByteArray& data);

private:
    Ui::ContactsPaneWidget *ui;
};

#endif // CONTACTSPANEWIDGET_H
