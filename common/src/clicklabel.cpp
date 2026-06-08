#include "clicklabel.h"

#include <QMouseEvent>

ClickLabel::ClickLabel(QWidget *parent)
    : QLabel(parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

void ClickLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}