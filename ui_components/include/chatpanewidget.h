#ifndef CHATPANEWIDGET_H
#define CHATPANEWIDGET_H

#include <QWidget>

namespace Ui {
class ChatPaneWidget;
}

class ChatPaneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPaneWidget(QWidget *parent = nullptr);
    ~ChatPaneWidget();

signals:
    void SIG_AddFriend();

private:
    Ui::ChatPaneWidget *ui;
};

#endif // CHATPANEWIDGET_H
