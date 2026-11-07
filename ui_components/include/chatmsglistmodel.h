#ifndef CHATMSGLISTMODEL_H
#define CHATMSGLISTMODEL_H

#include <QAbstractItemModel>

enum class ChatMessageType {
    Receiver = 0,
    Sender
};

enum class ChatContentType {
    Text,
    Image,
    TextAndImage
};

struct ChatMessage {
    QString time;
    ChatContentType contentType;
    QString text;
    QString imagePath;
    ChatMessageType msgType;

};

class ChatMsgListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        TimeRole =  Qt::UserRole + 1,
        MessageTypeRole,
        ContentTypeRole,
        TextRole,
        ImagePathRole
    };

    explicit ChatMsgListModel(QObject *parent = nullptr);
    ~ChatMsgListModel() = default;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void SetMessage(const QList<ChatMessage> &message);
    void AppendMessage(const ChatMessage &message);

private:
    QList<ChatMessage> m_messages;
};

#endif // CHATMSGLISTMODEL_H
