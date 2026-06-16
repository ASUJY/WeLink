#include "label.h"

Label::Label(QWidget *parent)
    : QLabel{parent}
{
    this->setWordWrap(true);
}

void Label::SetText(QString text) {
    QLabel::setText(text);

    double maxWidth = this->maximumWidth();
    auto width = fontMetrics().boundingRect(text).width();
    auto height = fontMetrics().height();
    QRect rect = geometry();

    if (width + 20 >= maxWidth) {
        maxWidth = maxWidth - 20;
        auto ratio = ceil(width / maxWidth);
        rect.setHeight(height * ratio + 30);
        rect.setWidth(maxWidth);
    } else {
        rect.setWidth(width + 20);
    }
    setGeometry(rect);
    emit SIG_LabelSizeChanged(rect);
}
