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
    explicit SenderMsgItem(QWidget *parent = nullptr, const QString& imageUrl = QString()) noexcept;
    void SetMessage(const QString &msg);

private:
    QHBoxLayout *m_layout;
    QPushButton *m_btnHead;
    BubbleLabel *m_bubble;

    static const int HEAD_SIZE;
    static const int HEAD_ICON_SIZE;
    static const int LAYOUT_MARGIN_H;
    static const int LAYOUT_MARGIN_V;
    static const int LAYOUT_SPACING;
};

#endif // SENDERMSGITEM_H