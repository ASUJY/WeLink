#ifndef CHATPANELISTWIDGET_H
#define CHATPANELISTWIDGET_H

#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>

class ChatPaneListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatPaneListWidget(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent *event) override;
};

#endif // CHATPANELISTWIDGET_H
