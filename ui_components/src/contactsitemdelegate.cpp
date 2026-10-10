#include "ContactsItemDelegate.h"
#include "contactstreemodel.h"
#include <QPainter>

namespace {
const QString ICON_ARROW_DOWN  = ":/resource/icon/menu/aio_arrow_down.png";
const QString ICON_ARROW_RIGHT = ":/resource/icon/menu/aio_arrow_right.png";

constexpr int kGroupHeight  = 30;
constexpr int kChildHeight  = 60;
constexpr int kIconSize     = 40;
constexpr int kIconMargin   = 10;
constexpr int kTextMargin   = 70;   // 文字左边距（头像右缘 + 间距）}
}


ContactsItemDelegate::ContactsItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ContactsItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);   // 开启抗锯齿，让图形边缘更平滑

    const int type = index.data(ContactsTreeModel::ItemTypeRole).toInt();

    // 分组项
    if (type == static_cast<int>(ContactsItemType::Group)) {
        PaintGroupItem(painter, option, index);
        painter->restore();
        return;
    }

    // 绘制子项
    PaintItem(painter, option, index);

    painter->restore();
}

void ContactsItemDelegate::PaintGroupItem(QPainter *painter,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const {
    const QRect rect = option.rect; // 获取当前项在视图中的矩形区域（位置和大小），即当前项要绘制的矩形区域

    // 填充背景
    painter->fillRect(rect, QColor("#FFFFFF"));

    // 展开/折叠箭头:直接从视图获取展开状态
    const bool expanded = option.state & QStyle::State_Open;
    const QPixmap arrow(expanded ? ICON_ARROW_DOWN : ICON_ARROW_RIGHT);
    if (!arrow.isNull()) {
        const int a = 12;
        const int y = rect.y() + (rect.height() - a) / 2;
        painter->drawPixmap(rect.x() + 8, y, a, a, arrow);
    }

    // 分组标题
    painter->setPen(QColor("#000000"));
    QFont f = painter->font();
    f.setPixelSize(12);
    painter->setFont(f);
    // 绘制分组标题：左边留 30 像素，右边留 10 像素，垂直居中、左对齐，文本来自 NameRole
    painter->drawText(rect.adjusted(30, 0, -10, 0),
                      Qt::AlignVCenter | Qt::AlignLeft,
                      index.data(ContactsTreeModel::NameRole).toString());
}

void ContactsItemDelegate::PaintItem(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const {
    const QRect rect = option.rect;

    // ---------- 子项 ----------
    // 背景（选中态给个浅蓝）
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, QColor("#C7E3FF"));
    } else {
        painter->fillRect(rect, QColor("#D9D8D7"));
    }

    // 头像
    const QString iconPath = index.data(ContactsTreeModel::HeadIconRole).toString();
    if (!iconPath.isEmpty()) {
        QPixmap pix(iconPath);
        if (!pix.isNull()) {
            painter->drawPixmap(rect.x() + kIconMargin,
                                rect.y() + kIconMargin,
                                kIconSize, kIconSize, pix);
        }
    }

    // 名称
    painter->setPen(QColor("#000000"));
    QFont f = painter->font();
    f.setPixelSize(12);
    painter->setFont(f);
    painter->drawText(QRect(rect.x() + kTextMargin,
                            rect.y(),
                            rect.width() - kTextMargin,
                            rect.height()),
                      Qt::AlignVCenter | Qt::AlignLeft,
                      index.data(ContactsTreeModel::NameRole).toString());
}

QSize ContactsItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    const int type = index.data(ContactsTreeModel::ItemTypeRole).toInt();
    const int h = (type == static_cast<int>(ContactsItemType::Group))
                      ? kGroupHeight : kChildHeight;
    return QSize(option.rect.width(), h);
}