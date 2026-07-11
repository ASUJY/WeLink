#ifndef RECEIVERMSGITEM_H
#define RECEIVERMSGITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "BubbleLabel.h"

class ReceiverMsgItem : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiverMsgItem(QWidget *parent = nullptr, QString imageUrl = "");
    void SetMessage(const QString &msg);
    void SetHeadPixmap(const QPixmap &pix);

private:
    QHBoxLayout *m_layout;
    QPushButton *m_btnHead;
    BubbleLabel *m_bubble;
};

#endif // RECEIVERMSGITEM_H