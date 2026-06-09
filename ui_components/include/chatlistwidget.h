#ifndef CHATLISTWIDGET_H
#define CHATLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ChatListWidget;
}

class ChatListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListWidget(QWidget *parent = nullptr);
    ~ChatListWidget();

signals:
    void SIG_AddFriend();

private:
    Ui::ChatListWidget *ui;
};

#endif // CHATLISTWIDGET_H
