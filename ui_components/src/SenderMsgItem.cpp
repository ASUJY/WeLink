#include "SenderMsgItem.h"

const int SenderMsgItem::HEAD_SIZE        = 36;
const int SenderMsgItem::HEAD_ICON_SIZE   = 34;
const int SenderMsgItem::LAYOUT_MARGIN_H  = 10;
const int SenderMsgItem::LAYOUT_MARGIN_V  = 6;
const int SenderMsgItem::LAYOUT_SPACING   = 10;

SenderMsgItem::SenderMsgItem(QWidget *parent, const QString& imageUrl)noexcept : QWidget(parent) {
    setStyleSheet("background-color: transparent;");
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(LAYOUT_MARGIN_H, LAYOUT_MARGIN_V, LAYOUT_MARGIN_H ,LAYOUT_MARGIN_V);
    m_layout->setSpacing(LAYOUT_SPACING);

    m_btnHead = new QPushButton(this);
    m_btnHead->setFixedSize(HEAD_SIZE, HEAD_SIZE);
    if (!imageUrl.isEmpty()) {
        m_btnHead->setIcon(QIcon(imageUrl));
    }
    m_btnHead->setIconSize(QSize(HEAD_ICON_SIZE, HEAD_ICON_SIZE));
    m_btnHead->setStyleSheet("border-radius:18px; border:none;");

    m_bubble = new BubbleLabel(this);
    m_bubble->SetBubbleType(BubbleLabel::Sender);

    // 左侧弹簧，气泡靠右
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addSpacerItem(spacer);
    m_layout->addWidget(m_bubble);
    m_layout->addWidget(m_btnHead);
}

void SenderMsgItem::SetMessage(const QString &msg) {
    m_bubble->setText(msg);
    adjustSize();
}