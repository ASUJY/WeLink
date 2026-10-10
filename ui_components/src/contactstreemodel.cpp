#include "contactstreemodel.h"

ContactsTreeModel::ContactsTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{}

ContactsTreeModel::~ContactsTreeModel() = default;

QModelIndex ContactsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // 因为是联系人列表项的树状结构，只有第0列
    // 只支持第 0 列，列不是 0 或行号小于 0，返回无效索引
    if (column != 0 || row < 0) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        if (row >= static_cast<int>(m_groups.size())) {
            return QModelIndex();
        }
        return createIndex(row, column, m_groups[row].get());
    }

    Node* parentNode = static_cast<Node*>(parent.internalPointer());
    if (!parentNode || !parentNode->IsGroup()) {
        return QModelIndex();
    }
    if (row >= static_cast<int>(parentNode->m_children.size())) {
        return QModelIndex();
    }

    return createIndex(row, column, parentNode->m_children[row].get());
}

int ContactsTreeModel::RowOf(const Node* node) const {
    if (!node) {
        return -1;
    }

    if (!node->m_parent) {
        for (int i = 0; i < static_cast<int>(m_groups.size()); ++i) {
            if (m_groups[i].get() == node) {
                return i;
            }
        }
        return -1;
    }

    const auto& siblings = node->m_parent->m_children;
    for (int i = 0; i < static_cast<int>(siblings.size()); ++i) {
        if (siblings[i].get() == node) {
            return i;
        }
    }
    return -1;
}

// 返回给定 子项模型索引(QModelIndex) 的 父项模型索引(QModelIndex)
QModelIndex ContactsTreeModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex();   // 用户给出的模型索引无效则返回空索引
    }

    Node* node = static_cast<Node*>(child.internalPointer());
    if (!node || !node->m_parent) {
        return QModelIndex();
    }

    const int groupRow = RowOf(node->m_parent);
    if (groupRow < 0) {
        return QModelIndex();
    }

    return createIndex(groupRow, 0, node->m_parent);
}

// 返回父节点下的节点的行数
int ContactsTreeModel::rowCount(const QModelIndex& parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        return static_cast<int>(m_groups.size());
    }

    Node* node = static_cast<Node*>(parent.internalPointer());
    if (!node || !node->IsGroup()) {
        return 0;
    }

    return static_cast<int>(node->m_children.size());
}

// 返回父节点下的节点列数，树状模型每个父节点下只有一列
int ContactsTreeModel::columnCount(const QModelIndex&) const {
    return 1;
}

// 返回索引所指向项中，指定角色下存储的数据。
QVariant ContactsTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    Node* node = static_cast<Node*>(index.internalPointer());
    if (!node || !node->m_item) {
        return QVariant();
    }

    auto item = node->m_item;

    switch (role) {
    case Qt::DisplayRole:
    case NameRole:
        return item->GetItemType() == ContactsItemType::Group
                   ? item->GetGroupName() : item->GetItemName();
    case IdRole:
        return QVariant::fromValue<qlonglong>(item->GetItemId());
    case HeadIconRole:
        return item->GetHeadIcon();
    case GroupNameRole:
        return item->GetGroupName();
    case ItemTypeRole:
        return static_cast<int>(item->GetItemType());
    case FriendStateRole:
        return static_cast<int>(item->GetItemState());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ContactsTreeModel::roleNames() const
{
    return {
            { NameRole,        "name"        },
            { IdRole,          "id"          },
            { HeadIconRole,    "headIcon"    },
            { GroupNameRole,   "groupName"   },
            { ItemTypeRole,    "itemType"    },
            { FriendStateRole, "friendState" },
            };
}

ContactsTreeModel::Node* ContactsTreeModel::FindGroupNode(const QString& name) const {
    auto it = m_groupsByName.find(name);
    return it == m_groupsByName.end() ? nullptr : it.value();
}

void ContactsTreeModel::InsertItem(std::unique_ptr<ContactsItem> item) {
    if (!item) {
        return;
    }

    // 分组项
    if (item->GetItemType() == ContactsItemType::Group) {
        const QString groupName = item->GetGroupName();
        if (groupName.isEmpty() || m_groupsByName.contains(groupName)) {
            return;     // 已有同名分组，忽略
        }

        const int row = static_cast<int>(m_groups.size());

        // 开始行插入操作，在最后添加一行
        beginInsertRows(QModelIndex(), row, row);

        // 在分组项列表末尾插入分组节点，同时记录到m_groupsByName，方便快速查到分组节点
        auto node = std::make_unique<Node>();
        node->m_item = std::move(item);
        Node* nodeRaw = node.get();
        m_groups.push_back(std::move(node));
        m_groupsByName.insert(groupName, nodeRaw);

        endInsertRows();    // 结束行插入操作
        return;
    }

    // 子项：找到目标分组
    const QString targetGroup = item->GetGroupName();
    Node* group = FindGroupNode(targetGroup);
    if (!group) {
        // 分组不存在则自动创建
        auto groupItem = std::make_unique<ContactsItem>();
        groupItem->SetItemType(ContactsItemType::Group);
        groupItem->SetGroupName(targetGroup);
        InsertItem(std::move(groupItem));
        group = FindGroupNode(targetGroup);
        if (!group) {
            return;
        }
    }

    // 把联系人子项插入该分组的子节点列表末尾，并设置父节点。
    const int groupRow = RowOf(group);
    const QModelIndex gi = index(groupRow, 0, QModelIndex());

    const int row = static_cast<int>(group->m_children.size());
    beginInsertRows(gi, row, row);

    auto child = std::make_unique<Node>();
    child->m_item = std::move(item);
    child->m_parent = group;
    group->m_children.push_back(std::move(child));

    endInsertRows();
}

void ContactsTreeModel::UpdateItemStatus(const QString& groupname, int64_t id, FriendState state) {
    Node* group = FindGroupNode(groupname);
    if (!group) {
        return;
    }

    int row = 0;
    for (auto it = group->m_children.begin(); it != group->m_children.end(); ++it) {
        if ((*it)->m_item && (*it)->m_item->GetItemId() == id) {
            (*it)->m_item->SetItemState(state);
            const QModelIndex gi = index(RowOf(group), 0, QModelIndex());
            const QModelIndex ci = index(row, 0, gi);
            emit dataChanged(ci, ci, {FriendStateRole});
            return;
        }
        ++row;
    }
}

std::shared_ptr<ContactsItem>
ContactsTreeModel::ItemFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return nullptr;
    }

    Node* node = static_cast<Node*>(index.internalPointer());
    return node ? node->m_item : nullptr;
}