#ifndef SENDERMSGITEM_H
#define SENDERMSGITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "BubbleLabel.h"

class SenderMsgItem : public QWidget
{
    Q_OBJECT
public:
    explicit SenderMsgItem(QWidget *parent = nullptr, QString imageUrl = "");
    void SetMessage(const QString &msg);

private:
    QHBoxLayout *m_layout;
    QPushButton *m_btnHead;
    BubbleLabel *m_bubble;
};

#endif // SENDERMSGITEM_H