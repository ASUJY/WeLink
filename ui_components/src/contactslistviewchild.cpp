#include "contactslistviewchild.h"

#include <QPainter>

ContactsListViewChild::ContactsListViewChild(int64_t id,
                                             const QString& name,
                                             const QString& headIcon,
                                             const QString& groupname,
                                             QWidget *parent) noexcept
    : m_userid(id), m_username(name), m_headIcon(headIcon), m_groupname(groupname) {}

ContactsListViewChild::~ContactsListViewChild() {

}

void ContactsListViewChild::paintEvent(QPaintEvent *) {
    qDebug() << "ContactsListViewChild::paintEvent";

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(this->rect(), QColor("#D9D8D7"));
    // 绘制头像
    QPixmap pixmap(m_headIcon);
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
    painter.drawText(70, 0, w - 70, h / 2 - 2, Qt::AlignBottom, m_username);
}

void ContactsListViewChild::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        emit SIG_ItemDidSelected(m_userid, m_groupname);
    }
    QWidget::mousePressEvent(event);
}