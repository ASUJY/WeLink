#include "chatmsglistmodel.h"

ChatMsgListModel::ChatMsgListModel(QObject *parent)
    : QAbstractItemModel{parent}
{}

QModelIndex ChatMsgListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row >= m_messages.size()
        || column != 0) {
        return {};
    }

    return createIndex(row, column);
}

QModelIndex ChatMsgListModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return {};
}

int ChatMsgListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;   // 列表只有一列，没有子项
    }
    return static_cast<int>(m_messages.size());
}

int ChatMsgListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant ChatMsgListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_messages.size()) {
        return QVariant();
    }

    const ChatMessage &message = m_messages.at(index.row());

    switch(role) {
    case TimeRole:
        return message.time;
    case MessageTypeRole:
        return static_cast<int>(message.msgType);
    case ContentTypeRole:
        return static_cast<int>(message.contentType);
    case TextRole:
        return message.text;
    case ImagePathRole:
        return message.imagePath;
    default:
        return {};
    }
}

void ChatMsgListModel::SetMessage(const QList<ChatMessage> &message) {
    beginResetModel();
    m_messages = message;
    endResetModel();
}

void ChatMsgListModel::AppendMessage(const ChatMessage &message) {
    const int row = m_messages.size();
    beginInsertRows(QModelIndex(), row, row);
    m_messages.append(message);
    endInsertRows();
}
