#include "contactslistviewchild.h"

#include <QPainter>

ContactsListViewChild::ContactsListViewChild(QWidget *parent) {}

ContactsListViewChild::~ContactsListViewChild() {
    // if (m_item) {
    //     delete m_item;
    //     m_item = nullptr;
    // }
}

void ContactsListViewChild::paintEvent(QPaintEvent *) {
    qDebug() << "ContactsListViewChild::paintEvent";
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(this->rect(), QColor("#D9D8D7"));
    // 绘制头像
    QPixmap pixmap(m_item->GetHeadIcon());
    painter.drawPixmap(10, 10, 40, 40, pixmap);

    int w = this->rect().width();
    int h = this->rect().height();

    // 绘制名称
    QFont font("宋体", 14);
    QPen pen;
    pen.setColor(QColor("#000000"));
    font.setPixelSize(12);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(70, 0, w - 70, h / 2 - 2, Qt::AlignBottom, m_item->GetItemName());
}

void ContactsListViewChild::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        // User user;
        // user.SetUserName(m_item->GetItemName().toStdString());
        emit SIG_ItemDidSelected(this);
    }
}