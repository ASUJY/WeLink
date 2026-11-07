#ifndef CHATMSGDELEGATE_H
#define CHATMSGDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>
#include <chatmsglistmodel.h>

class ChatMsgDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ChatMsgDelegate(QObject *parent = nullptr);
    ~ChatMsgDelegate() = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void SetHeader(const QString &senderAvatar, const QString &receiverAvatar);

private:
    struct Layout {
        QRect timeRect;     // 时间边框
        QRect avatarRect;   // 头像边框
        QRect bubbleRect;   // 气泡边框
        QRect textRect;     // 文字边框
        QRect imageRect;    // 图片边框
        int textLength = 0;
    };

    auto CalculateLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const -> Layout;
    void CalculateBubbleWidths(const QString &text, bool sender, const int availableBubbleWidth,
                               int &bubbleWidth, int &textWidth) const;

    void LayoutRect(const QRect& itemRect, Layout &layout,
                    const int innerContentHeight, const int bubbleWidth,  bool sender) const;
    void LayoutTextRect(const QRect& itemRect, Layout &layout,
                        const int textWidth,const int textHeight, bool sender) const;
    void LayoutImageRect(Layout &layout, bool hasText, const int textHeight, bool hasImage, const QSize& imageSize, bool sender) const;

    void DrawBubble(QPainter *painter, const QRect &rect, bool sender) const;
    void DrawTime(QPainter *painter, const QRect &rect, const QString& timee, bool sender) const;
    void DrawAvatar(QPainter *painter, const QRect &rect, bool sender) const;
    void DrawText(QPainter *painter, const QRect &rect, const ChatContentType& contentType, const QString& text) const;
    void DrawImage(QPainter *painter, const QRect &rect, const ChatContentType& contentType, const QString& imagePath) const;

private:
    // 发送方和接收方的头像路径。
    QString m_senderAvatar;
    QString m_receiverAvatar;
    mutable QPixmap m_senderAvatarPixmap;
    mutable QPixmap m_receiverAvatarPixmap;

    mutable QTextDocument m_doc;
};

#endif // CHATMSGDELEGATE_H
