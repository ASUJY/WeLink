#include "searchpopup.h"
#include <QApplication>
#include <QMouseEvent>
SearchPopup::SearchPopup(QWidget* parent) : QFrame(parent, Qt::Tool | Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(250, 200);
    setStyleSheet(
        "QFrame {"
        "    background-color: white;"
        "    border: 1px solid #DDDDDD;"
        "    border-radius: 6px;"
        "}"
        );
}

SearchPopup::~SearchPopup()
{
}

void SearchPopup::ShowBelow(QWidget *widget) {
    if (!widget) {
        return;
    }

    // 获取控件左下角的全局坐标
    QPoint pos = widget->mapToGlobal(QPoint(0, widget->height()));

    this->move(pos);
    this->show();
}

void SearchPopup::HidePopup() {
    this->hide();
}