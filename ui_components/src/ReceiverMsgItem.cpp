#include "ReceiverMsgItem.h"

const int ReceiverMsgItem::HEAD_SIZE = 36;
const int ReceiverMsgItem::HEAD_ICON_SIZE = 34;
const int ReceiverMsgItem::LAYOUT_MARGIN_H = 10;
const int ReceiverMsgItem::LAYOUT_MARGIN_V = 6;
const int ReceiverMsgItem::LAYOUT_SPACING = 10;

ReceiverMsgItem::ReceiverMsgItem(QWidget *parent, const QString& imageUrl) noexcept
    : QWidget(parent)
{
    setStyleSheet("background-color: transparent;");
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(LAYOUT_MARGIN_H, LAYOUT_MARGIN_V, LAYOUT_MARGIN_H, LAYOUT_MARGIN_V);
    m_layout->setSpacing(LAYOUT_SPACING);

    // 圆形头像按钮
    m_btnHead = new QPushButton(this);
    m_btnHead->setFixedSize(HEAD_SIZE, HEAD_SIZE);
    if (!imageUrl.isEmpty()) {
        m_btnHead->setIcon(QIcon(imageUrl));
    }
    m_btnHead->setIconSize(QSize(HEAD_ICON_SIZE, HEAD_ICON_SIZE));
    m_btnHead->setStyleSheet("border-radius:18px; border:none;");

    // 白色气泡
    m_bubble = new BubbleLabel(this);
    m_bubble->SetBubbleType(BubbleLabel::Receiver);

    // 右侧弹簧，气泡靠左
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_layout->addWidget(m_btnHead);
    m_layout->addWidget(m_bubble);
    m_layout->addSpacerItem(spacer);
}

void ReceiverMsgItem::SetMessage(const QString &msg)
{
    m_bubble->setText(msg);
    adjustSize(); // 自动适配整体控件大小
}

void ReceiverMsgItem::SetHeadPixmap(const QPixmap &pix)
{
    m_btnHead->setIcon(QIcon(pix));
}