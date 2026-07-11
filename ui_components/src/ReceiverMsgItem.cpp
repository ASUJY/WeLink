#include "ReceiverMsgItem.h"

ReceiverMsgItem::ReceiverMsgItem(QWidget *parent, QString imageUrl)
    : QWidget(parent)
{
    setStyleSheet("background-color: transparent;");
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(10, 6, 10, 6);
    m_layout->setSpacing(10);

    // 圆形头像按钮
    m_btnHead = new QPushButton;
    m_btnHead->setFixedSize(36, 36);
     m_btnHead->setIcon(QIcon(imageUrl));
    m_btnHead->setIconSize(QSize(34, 34));
    m_btnHead->setStyleSheet("border-radius:18px; border:none;");

    // 白色气泡
    m_bubble = new BubbleLabel;
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