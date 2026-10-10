#ifndef CONTACTSTREEMODEL_H
#define CONTACTSTREEMODEL_H

#include <QAbstractItemModel>

#include "contactsitem.h"

class ContactsTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        IdRole,
        HeadIconRole,
        GroupNameRole,
        ItemTypeRole,
        FriendStateRole
    };

    explicit ContactsTreeModel(QObject *parent = nullptr);
    ~ContactsTreeModel() override;

    /**
     * 继承 QAbstractItemModel 必须实现5个接口index()、parent()、rowCount()、
     * columnCount()、data()；其它接口按需重写
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 对外业务接口
    // 添加一个列表项到TreeView视图中
    void InsertItem(std::unique_ptr<ContactsItem> item);
    // 修改TreeView视图中某个列表的状态
    void UpdateItemStatus(const QString& groupname, int64_t id, FriendState state);
    // 按 QModelIndex 取回完整的 ContactsItem（分组/子项通用）
    std::shared_ptr<ContactsItem> ItemFromIndex(const QModelIndex& index) const;

private:
    /*
     * 内部节点：直接持有 ContactsItem*
     * - 分组节点：m_item 是 Group 类型，m_children 是子节点
     * - 子节点：m_item 是 Item 类型，m_parent 指向分组节点
     **/
    struct Node {
        std::shared_ptr<ContactsItem> m_item;
        Node* m_parent = nullptr;                         // 仅子节点使用
        std::vector<std::unique_ptr<Node>> m_children;    // 仅分组节点使用

        bool IsGroup() const {
            return m_item && m_item->GetItemType() == ContactsItemType::Group;
        }
    };

    int RowOf(const Node* node) const;
    Node* FindGroupNode(const QString& name) const;

    // 一级：分组节点
    std::vector<std::unique_ptr<Node>> m_groups;    // 模型提供给QTreeView的数据，即QTreeView展示的都是m_groups的数据
    QHash<QString, Node*> m_groupsByName;
};

#endif // CONTACTSTREEMODEL_H
