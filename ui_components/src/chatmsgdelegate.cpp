#include "chatmsgdelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QtMath>
#include <QPainterPath>

namespace {
    // QListView中item内部的控件和item边界的距离
    constexpr int kMarginH = 10;
    constexpr int kMarginV = 6;

    // 时间框
    constexpr int kTimeHeight = 18;
    constexpr int kTimeToBubbleGap = 4;

    // 头像框
    constexpr int kHeadSize = 36;        // 头像所占的布局尺寸。
    constexpr int kHeadIconSize = 34;    // 头像图片的实际绘制尺寸
    constexpr int kSpacing = 10;         // 头像与气泡之间的间距。

    // 聊天气泡框
    constexpr int kMinBubbleWidth = 40;
    constexpr int kMaxBubbleWidth = 430;    // 气泡框允许的最大宽度
    // 气泡框内部的文字等距离气泡框边界的距离
    constexpr int kPaddingX = 12;
    constexpr int kPaddingY = 8;

    constexpr int kBubbleRadius = 8; // 气泡圆角大小
    constexpr int kTriangleSize = 6; // 气泡尖角的三角尺寸

    // 图片缩放时允许的最大宽高
    constexpr int kMaxImageWidth = 230;
    constexpr int kMaxImageHeight = 240;
    constexpr int kImageToTextGap = 6;


    QFont BubbleFont()
    {
        QFont font(QStringLiteral("Microsoft YaHei"));
        font.setPixelSize(14);
        return font;
    }

    QFont timeFont()
    {
        QFont font(QStringLiteral("Microsoft YaHei"));
        font.setPixelSize(14);
        return font;
    }

    // 根据给定宽度计算文字换行后的总高度
    int WrappedTextHeight(QTextDocument &doc, const QString &text, int width)
    {
        doc.setPlainText(text);
        doc.setTextWidth(qMax(1, width));   // 设置排版宽度；宽度至少为 1，避免无效值
        return qCeil(doc.size().height());  // 返回排版后的高度，向上取整，确保有足够空间显示文字。
    }

    // 读取图片，并计算它在给定最大宽高范围内、保持原始宽高比时的显示尺寸。
    QSize ScaledImageSize(const QString &path, int maxWidth, int maxHeight)
    {
        if (path.isEmpty())
            return {};

        QPixmap pixmap(path);
        if (pixmap.isNull())
            return {};

        // 将图片尺寸缩放到最大宽高范围内并保持宽高比，返回计算后的尺寸。
        return pixmap.size().scaled(QSize(maxWidth, maxHeight), Qt::KeepAspectRatio);
    }
}   // namespace


ChatMsgDelegate::ChatMsgDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_doc.setDefaultFont(BubbleFont());
    m_doc.setDocumentMargin(0); // 去掉默认文档边距

    // 去掉段落上下边距
    QTextBlockFormat blockFormat;
    blockFormat.setTopMargin(0);
    blockFormat.setBottomMargin(0);
    blockFormat.setLeftMargin(0);
    blockFormat.setRightMargin(0);

    QTextCursor cursor(&m_doc);
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(blockFormat);
}

void ChatMsgDelegate::SetHeader(const QString &senderAvatar, const QString &receiverAvatar)
{
    m_receiverAvatar = receiverAvatar;
    m_senderAvatar = senderAvatar;
    m_receiverAvatarPixmap.load(m_receiverAvatar);
    m_senderAvatarPixmap.load(m_senderAvatar);
}

/**
 * @brief 根据文字长度和item的实际可用空间，计算消息气泡宽度 bubbleWidth 以及气泡内文字宽度 textWidth。
 * @param text
 * @param sender
 * @param availableBubbleWidth
 * @param bubbleWidth
 * @param textWidth
 */
void ChatMsgDelegate::CalculateBubbleWidths(const QString &text, bool sender, int availableBubbleWidth,
                                            int &bubbleWidth, int &textWidth) const {
    const int textLeftMargin = kPaddingX + (sender ? 0 : kTriangleSize);
    const int textRightMargin = kPaddingX + (sender ? kTriangleSize : 0);

    // 文字不换行时的自然宽度。
    const QFontMetrics fm(BubbleFont());
    const int naturalTextWidth = fm.horizontalAdvance(text);

    // 宽消息最多占可用宽度；短消息按内容宽度显示。
    const int maxTextWidth = qMax(1, availableBubbleWidth - textLeftMargin - textRightMargin);
    textWidth = qMax(1, qMin(naturalTextWidth, maxTextWidth));
    bubbleWidth = qMin(availableBubbleWidth, textWidth + textLeftMargin + textRightMargin);
}

/**
 * @brief 计算QListView中每一个item的建议尺寸。
 * 设定内容的宽度，当内容很少时，则按照实际内容长度展示，
 * 当内容很多时，则设定最大宽度，内容自动换行，当聊天窗口被压缩时，聊天气泡也自动被压缩，即宽度变小，并且内容自动换行
 * 根据消息是文字、图片还是两者都有，累计内容高度，并加上气泡内边距、时间区域和行间留白。
 *
 * @param option
 * @param index
 * @return
 */
QSize ChatMsgDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
    // Get content
    const QString text = index.data(ChatMsgListModel::TextRole).toString();
    const QString imagePath = index.data(ChatMsgListModel::ImagePathRole).toString();
    const auto contentType = static_cast<ChatContentType>(index.data(ChatMsgListModel::ContentTypeRole).toInt());
    const bool sender = index.data(ChatMsgListModel::MessageTypeRole).toInt() == static_cast<int>(ChatMessageType::Sender);

    const int  actualBubbleWidth = option.rect.width() - 2 * kMarginH - kHeadSize - kSpacing;
    const int availableBubbleWidth = qMin(kMaxBubbleWidth, qMax(kMinBubbleWidth, actualBubbleWidth));

    int bubbleWidth = 0;
    int textWidth = 0;
    CalculateBubbleWidths(text, sender, availableBubbleWidth, bubbleWidth, textWidth);

    int contentHeight = 0;
    bool hasText = false;
    bool hasImage = false;

    if (contentType == ChatContentType::Text || contentType == ChatContentType::TextAndImage) {
        contentHeight += WrappedTextHeight(m_doc, text, textWidth);
        hasText = contentHeight > 0;
    }

    if (contentType == ChatContentType::Image || contentType == ChatContentType::TextAndImage) {
        const QSize image = ScaledImageSize(imagePath, kMaxImageWidth, kMaxImageHeight);
        if (!image.isEmpty()) {
            if (contentHeight > 0) {
                contentHeight += kImageToTextGap;
            }
            contentHeight += image.height();
            hasImage = true;
        }
    }

    if (!hasText && !hasImage) {
        contentHeight = 1;
    }

    const int bubbleHeight = contentHeight + 2 * kPaddingY;
    const int messageHeight = qMax(kHeadSize, bubbleHeight);
    const int actualMsgHeight = 2 * kMarginV + kTimeHeight + kTimeToBubbleGap + messageHeight;

    // 返回QListView中每一项的宽度和高度，聊天消息中的宽度一般不变，主要变的是高度
    return QSize(option.rect.width(), actualMsgHeight);
}

void ChatMsgDelegate::LayoutRect(const QRect& itemRect, Layout &layout, const int innerContentHeight,
                               const int bubbleWidth, bool sender) const {
    const int bubbleHeight = innerContentHeight + 2 * kPaddingY;
    const int contentTop = itemRect.top() + kMarginV + kTimeHeight + kTimeToBubbleGap; // 计算消息内容区域的顶部位置。
    const int contentHeight = qMax(kHeadSize, bubbleHeight);   // 取头像高度和气泡高度中较大者

    if (sender) {
        // 绘制时间，在最上方
        const int timeRectLeft = itemRect.left();
        const int timeRectTop = itemRect.top() + kMarginV;
        const int timeRectWidth = itemRect.width() - kMarginH - kHeadSize - kSpacing - kTriangleSize;
        const int timeRectHeight = kTimeHeight;
        layout.timeRect = QRect(timeRectLeft, timeRectTop, timeRectWidth, timeRectHeight);

        // 绘制头像
        const int avatarRectLeft = itemRect.right() - kMarginH - kHeadSize;
        const int avatarRectTop = contentTop + (contentHeight - kHeadSize) / 2;
        layout.avatarRect = QRect(avatarRectLeft, avatarRectTop, kHeadSize, kHeadSize);

        // 绘制气泡
        const int bubbleRight = layout.avatarRect.left() - kSpacing;
        const int bubbleRectLeft = bubbleRight - bubbleWidth;
        const int bubbleRectTop = contentTop + (contentHeight - bubbleHeight) / 2;
        layout.bubbleRect = QRect(bubbleRectLeft, bubbleRectTop, bubbleWidth, bubbleHeight);
    } else {
        const int timeRectLeft = itemRect.left() + kMarginH + kHeadSize + kSpacing + kTriangleSize;
        const int timeRectTop = itemRect.top() + kMarginV;
        const int timeRectWidth = itemRect.width();
        const int timeRectHeight = kTimeHeight;
        layout.timeRect = QRect(timeRectLeft, timeRectTop, timeRectWidth, timeRectHeight);

        const int avatarRectLeft = itemRect.left() + kMarginH;
        const int avatarRectTop = contentTop + (contentHeight - kHeadSize) / 2;
        layout.avatarRect = QRect(avatarRectLeft, avatarRectTop, kHeadSize, kHeadSize);

        const int bubbleRectLeft = layout.avatarRect.right() + kSpacing;
        const int bubbleRectTop = contentTop + (contentHeight - bubbleHeight) / 2;
        layout.bubbleRect = QRect(bubbleRectLeft, bubbleRectTop, bubbleWidth, bubbleHeight);
    }
}

void ChatMsgDelegate::LayoutTextRect(const QRect& itemRect, Layout &layout,
                  const int textWidth,const int textHeight, bool sender) const {
    const int textLeftMargin = kPaddingX + (sender ? 0 : kTriangleSize);
    const int textRectLeft = layout.bubbleRect.left() + textLeftMargin;
    const int textRectTop = layout.bubbleRect.top() + kPaddingY;
    layout.textRect = QRect(textRectLeft, textRectTop, textWidth, textHeight);
}

void  ChatMsgDelegate::LayoutImageRect(Layout &layout,
                                    bool hasText, const int textHeight,
                                    bool hasImage, const QSize& imageSize, bool sender) const {
     const int textLeftMargin = kPaddingX + (sender ? 0 : kTriangleSize);
    int imageY = layout.bubbleRect.top() + kPaddingY;
    if (hasText) {
        imageY += textHeight + (hasImage ? 6 : 0);
    }
    layout.imageRect = QRect(layout.bubbleRect.left() + textLeftMargin, imageY, imageSize.width(), imageSize.height());
}

/**
 * @brief 根据消息类型、文字/图片内容和行区域，计算时间、头像、气泡、文字及图片各自的矩形位置与尺寸，供绘制和点击处理使用。
 * @param option
 * @param index
 * @return
 */
auto ChatMsgDelegate::CalculateLayout(const QStyleOptionViewItem &option,
    const QModelIndex &index) const -> Layout
{
    Layout result;
    const QRect itemRect = option.rect; // QListView中某一个item的显示区域

    // Get Item Content
    const auto contentType = static_cast<ChatContentType>(index.data(ChatMsgListModel::ContentTypeRole).toInt());
    const QString text = index.data(ChatMsgListModel::TextRole).toString();
    const QString imagePath = index.data(ChatMsgListModel::ImagePathRole).toString();
    const bool sender = index.data(ChatMsgListModel::MessageTypeRole).toInt() == static_cast<int>(ChatMessageType::Sender);

    // Calculate the width of different elements
    const int  actualBubbleWidth = itemRect.width() - 2 * kMarginH - kHeadSize - kSpacing;
    const int availableBubbleWidth = qMin(kMaxBubbleWidth, qMax(kMinBubbleWidth,  actualBubbleWidth));
    int bubbleWidth = 0;
    int textWidth = 0;
    CalculateBubbleWidths(text, sender, availableBubbleWidth, bubbleWidth, textWidth);

    // Calculate the height of textRect
    int textHeight = 0;
    if (contentType == ChatContentType::Text || contentType == ChatContentType::TextAndImage) {
        textHeight = WrappedTextHeight(m_doc, text, textWidth);
    }

    QSize imageSize;
    const int textLeftMargin = kPaddingX + (sender ? 0 : kTriangleSize);    // 聊天气泡框 的尖角一侧多留 triangle 像素。
    const int textRightMargin = kPaddingX + (sender ? kTriangleSize : 0);
    if (contentType == ChatContentType::Image || contentType == ChatContentType::TextAndImage) {
        imageSize = ScaledImageSize(imagePath, kMaxImageWidth, kMaxImageHeight);
        imageSize.setWidth(qMin(imageSize.width(), bubbleWidth - textLeftMargin - textRightMargin));
    }

    const bool hasText = textHeight > 0;
    const bool hasImage = !imageSize.isEmpty();

    int innerContentHeight = 0;
    if (hasText)
        innerContentHeight += textHeight;
    if (hasText && hasImage)
        innerContentHeight += 6;
    if (hasImage)
        innerContentHeight += imageSize.height();

    // 分配一条消息中不同元素的显示位置（布局头像，气泡框等在列表项中的显示位置）
    LayoutRect(itemRect, result, innerContentHeight, bubbleWidth, sender);
    LayoutTextRect(itemRect, result, textWidth, textHeight, sender);
    LayoutImageRect(result, hasText, textHeight, hasImage, imageSize, sender);

    result.textLength = text.size();
    return result;
}

/**
 * @brief 根据发送方/接收方身份，构造并填充不同颜色、带圆角和方向尖角的消息气泡。
 * @param painter
 * @param rect
 * @param sender
 */
void ChatMsgDelegate::DrawBubble(QPainter *painter,const QRect &rect,
                                 bool sender) const {
    QPainterPath path;
    QColor bubbleColor;

    // 消息发送方(我方)
    if (sender) {
        bubbleColor = QColor(143, 232, 102);
        QRect bubbleRect(rect.left(), rect.top(), rect.width() - kTriangleSize, rect.height());

        path.addRoundedRect(bubbleRect, kBubbleRadius, kBubbleRadius);
        path.moveTo(rect.right() - kTriangleSize + 1, rect.center().y() - kTriangleSize);
        path.lineTo(rect.right(), rect.center().y());
        path.lineTo(rect.right() - kTriangleSize + 1, rect.center().y() + kTriangleSize);
        path.closeSubpath();
    } else {
        bubbleColor = QColor(244, 244, 244);

        QRect bubbleRect(rect.left() + kTriangleSize, rect.top(), rect.width() - kTriangleSize, rect.height());

        path.addRoundedRect(bubbleRect, kBubbleRadius, kBubbleRadius);
        path.moveTo(rect.left() + kTriangleSize, rect.center().y() - kTriangleSize);
        path.lineTo(rect.left(), rect.center().y());
        path.lineTo(rect.left() + kTriangleSize, rect.center().y() + kTriangleSize);
        path.closeSubpath();
    }
    painter->fillPath(path, bubbleColor);        // 用背景色填充整个绘制路径（画出气泡背景）
}

/**
 * @brief 根据发送方/接收方身份，显示时间
 * @param painter
 * @param rect
 * @param time
 * @param sender
 */
void ChatMsgDelegate::DrawTime(QPainter *painter, const QRect &rect, const QString& timee, bool sender) const {
    painter->setFont(timeFont());
    painter->setPen(QColor("#999999"));
    if (sender) {
        painter->drawText(rect, Qt::AlignRight, timee);
    } else {
        painter->drawText(rect, Qt::AlignLeft, timee);
    }
}

void ChatMsgDelegate::DrawAvatar(QPainter *painter, const QRect &avatarRect, bool sender) const {
    // 画头像：图像绘制在圆形裁剪区域内。
    const QPixmap &avatar = sender ? m_senderAvatarPixmap : m_receiverAvatarPixmap;
    if (!avatar.isNull()) {
        const int iconRectLeft = avatarRect.center().x() - kHeadIconSize / 2;
        const int iconRectTop = avatarRect.center().y() - kHeadIconSize / 2;
        const QRect iconRect(iconRectLeft, iconRectTop, kHeadIconSize, kHeadIconSize);

        QPainterPath clip;
        clip.addEllipse(iconRect);
        painter->save();
        painter->setClipPath(clip);
        painter->drawPixmap(iconRect, avatar);
        painter->restore();
    }
}

void ChatMsgDelegate::DrawText(QPainter *painter, const QRect &textRect, const ChatContentType& contentType, const QString& text) const {
    // 画文字
    if (contentType == ChatContentType::Text || contentType == ChatContentType::TextAndImage) {
        m_doc.setPlainText(text);
        m_doc.setTextWidth(textRect.width());

        painter->save();
        painter->setPen(QColor("#333333"));
        painter->translate(textRect.topLeft());
        m_doc.drawContents(painter);
        painter->restore();
    }
}

void ChatMsgDelegate::DrawImage(QPainter *painter, const QRect &imageRect, const ChatContentType& contentType, const QString& imagePath) const {
    // 画图片
    if ((contentType == ChatContentType::Image || contentType == ChatContentType::TextAndImage) &&
        !imageRect.isEmpty()) {
        QPixmap image(imagePath);
        if (!image.isNull()) {
            painter->drawPixmap(imageRect, image);
        }
    }
}


/**
 * @brief 绘制一条消息：先画时间，再画发送方或接收方气泡，然后绘制文字和图片。
 * 文字若有选区，会用蓝色背景标出。
 * @param painter
 * @param option
 * @param index
 */
void ChatMsgDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Get Item Content
    const QString timee = index.data(ChatMsgListModel::TimeRole).toString();
    const QString text = index.data(ChatMsgListModel::TextRole).toString();
    const QString imagePath = index.data(ChatMsgListModel::ImagePathRole).toString();
    const auto contentType = static_cast<ChatContentType>(index.data(ChatMsgListModel::ContentTypeRole).toInt());
    const bool sender = index.data(ChatMsgListModel::MessageTypeRole).toInt() == static_cast<int>(ChatMessageType::Sender);

    const Layout layout = CalculateLayout(option, index);   // 计算好item中气泡，头像，时间的显示区域

    // 画时间
    DrawTime(painter, layout.timeRect, timee, sender);

    // 画头像
    DrawAvatar(painter, layout.avatarRect, sender);

    // 画气泡
    DrawBubble(painter, layout.bubbleRect, sender);

    // 画文字，支持按字符位置绘制选区高亮。
    DrawText(painter, layout.textRect, contentType, text);

    // 画图片
    DrawImage(painter, layout.imageRect, contentType, imagePath);

    painter->restore();
}