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

signals:
    void SIG_SendChatMsg(int64_t id, const QString& content);

public:
    explicit ChatMainWidget(QWidget *parent = nullptr);
    ~ChatMainWidget();

    void SetData(Friend *data);
    void SetStackedWidgettCurrentIndex(int index);

private slots:
    void SlotBtnSendMsgClicked();

private:
    Ui::ChatMainWidget *ui;
};

#endif // CHATMAINWIDGET_H
