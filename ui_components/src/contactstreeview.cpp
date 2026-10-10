#include "contactstreeview.h"
#include "contactsitemdelegate.h"

ContactsTreeView::ContactsTreeView(QWidget* parent)
    : QTreeView(parent) {
    m_model = new ContactsTreeModel(this);
    m_delegate = new ContactsItemDelegate(this);
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);

    this->setHeaderHidden(true);        // 隐藏表头
    this->setRootIsDecorated(false);    // 分组箭头由 delegate 自己画
    this->setIndentation(0);            // 子项不缩进
    this->setUniformRowHeights(false);  // 设置所有行不采用统一高度，因为分组项和子项高度不同；分组高度30/子项高度60，高度不固定
    this->setSelectionMode(QAbstractItemView::SingleSelection); // 单项选择（一次只能选一个列表项）
    this->setSelectionBehavior(QAbstractItemView::SelectRows);  // 设置选择行为为选中整行
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 每一项不允许编辑
    // this->setExpandsOnDoubleClick(false);                    // 设置双击项时不自动展开/折叠。
    this->setAnimated(true);                                    // 设置展开/折叠时有动画效果。
    this->setMouseTracking(true);                               // 启用鼠标跟踪，即使没按下鼠标也能接收鼠标移动事件。
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // 设置垂直滚动模式为按像素滚动，即平滑滚动。

    connect(this, &QTreeView::clicked, this, &ContactsTreeView::OnClicked);
}

void ContactsTreeView::InsertItem(std::unique_ptr<ContactsItem> item) {
    if (!item) return;

    m_model->InsertItem(std::move(item));

}

void ContactsTreeView::UpdateItemState(const QString& groupname, int64_t id, FriendState state) {
    m_model->UpdateItemStatus(groupname, id, state);
}

void ContactsTreeView::OnClicked(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }

    const int type = index.data(ContactsTreeModel::ItemTypeRole).toInt();

    // 分组项：切换展开/折叠
    if (type == static_cast<int>(ContactsItemType::Group)) {
        setExpanded(index, !isExpanded(index));
        return;
    }

    // 子项：通知外部
    // 子项：取回完整的 ContactsItem 发送出去
    auto item = m_model->ItemFromIndex(index);
    if (item) {
        emit SIG_ItemDidSelected(item);
    }
}