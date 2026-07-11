#include "SenderMsgItem.h"

SenderMsgItem::SenderMsgItem(QWidget *parent, QString imageUrl) : QWidget(parent) {
    setStyleSheet("background-color: transparent;");
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(10, 6, 10 ,6);
    m_layout->setSpacing(10);

    m_btnHead = new QPushButton;
    m_btnHead->setFixedSize(36, 36);
    m_btnHead->setIcon(QIcon(imageUrl));
    m_btnHead->setIconSize(QSize(34, 34));
    m_btnHead->setStyleSheet("border-radius:18px; border:none;");

    m_bubble = new BubbleLabel;
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