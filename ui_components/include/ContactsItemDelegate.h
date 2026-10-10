#ifndef CONTACTSITEMDELEGATE_H
#define CONTACTSITEMDELEGATE_H

#include <QStyledItemDelegate>

class ContactsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ContactsItemDelegate(QObject *parent = nullptr);

    // paint() 和 sizeHint() 是自定义外观时最核心的
    // paint()自定义列表项外观
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 自定义列表项QSize（高度和宽度）
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // createEditor()、setEditorData()、setModelData()、updateEditorGeometry()是自定义编辑操作时需要的，暂不需要
private:
    // 绘制分组项
    void PaintGroupItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    // 绘制子项
    void PaintItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CONTACTSITEMDELEGATE_H
