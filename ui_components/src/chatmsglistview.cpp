#include "chatmsglistview.h"

ChatMsgListView::ChatMsgListView(QWidget *parent)
{
    m_model = new ChatMsgListModel(this);
    m_delegate = new ChatMsgDelegate(this);

    this->setModel(m_model);
    this->setItemDelegate(m_delegate);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWordWrap(true);
    this->setMouseTracking(true);
    this->viewport()->setMouseTracking(true);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setResizeMode(QListView::Adjust);
}

void ChatMsgListView::SetMessage(const QList<ChatMessage>& msg)
{
    m_model->SetMessage(msg);
}

void ChatMsgListView::SetHeader(const QString &senderAvatar, const QString &receiverAvatar)
{
    m_delegate->SetHeader(senderAvatar, receiverAvatar);
}