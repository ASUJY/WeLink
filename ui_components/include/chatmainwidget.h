#ifndef CHATMAINWIDGET_H
#define CHATMAINWIDGET_H

#include <QWidget>

namespace Ui {
class ChatMainWidget;
}

class ChatMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMainWidget(QWidget *parent = nullptr);
    ~ChatMainWidget();

private:
    Ui::ChatMainWidget *ui;
};

#endif // CHATMAINWIDGET_H
