#ifndef CHATMAINWIDGET_H
#define CHATMAINWIDGET_H

#include <QWidget>
#include "friend.hpp"

namespace Ui {
class ChatMainWidget;
}

class ChatMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMainWidget(QWidget *parent = nullptr);
    ~ChatMainWidget();

    void SetData(Friend *data);

private:
    Ui::ChatMainWidget *ui;
};

#endif // CHATMAINWIDGET_H
