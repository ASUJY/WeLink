#ifndef CONTACTSITEM_H
#define CONTACTSITEM_H

#include <QString>
#include <QMap>
#include <memory>
#include "common.h"

enum class ContactsItemType : int {
    Group = 0,
    Item,
};

class ContactsItem
{
public:
    ContactsItem() = default;
    ContactsItem(const ContactsItem&) = delete;
    ContactsItem& operator=(const ContactsItem&) = delete;
    ContactsItem(ContactsItem&&) = default;
    ContactsItem& operator=(ContactsItem&&) = default;
    ~ContactsItem() = default;

    void SetGroupName(const QString& name) {m_groupName = name;}
    void SetIsOpen(bool isopen) {m_isOpen = isopen;}
    void SetItemName(const QString& name) {m_name = name;}
    void SetItemId(int64_t id) {m_id = id;}
    void SetHeadIcon(const QString& headIcon) {m_headIcon = headIcon;}
    void SetItemType(ContactsItemType type) {m_type = type;}
    void SetItemState(FriendState state) {m_state = state;}

    void AddChildItem(int64_t id, std::shared_ptr<ContactsItem> item) {
        m_childItems[id] = item;
    }
    void RemoveChildItem(int64_t id) {
        m_childItems.erase(id);
    }
    void ClearAllChildItems() {
        m_childItems.clear();
    }
    bool HasChildItem(int64_t id) const {
        return m_childItems.find(id) != m_childItems.cend();
    }

    QString GetGroupName() const {return m_groupName;}
    bool GetIsOpen() const {return m_isOpen;}
    QString GetItemName() const {return m_name;}
    int64_t GetItemId() const {return m_id;}
    ContactsItemType GetItemType() const {return m_type;}
    FriendState GetItemState() const {return m_state;}
    QString GetHeadIcon() const {return m_headIcon;}
    const std::map<int64_t, std::shared_ptr<ContactsItem>>& GetChildItems() const {return m_childItems;}
    std::shared_ptr<ContactsItem> GetChildItemById(int64_t id) const {
        auto it = m_childItems.find(id);
        if (it != m_childItems.end()) {
            return it->second;  // 拷贝shared_ptr，引用计数+1，调用方持有副本
        }
        return nullptr;
    }

private:
    QString m_name;         // 用户名
    int64_t m_id = -1;      // 用户ID
    QString m_headIcon;     // 用户头像
    FriendState m_state = FriendState::DONE;            // 列表项状态，表示朋友是否已添加或者是否已拒绝
    ContactsItemType m_type = ContactsItemType::Group;  // 列表项类型，分组项或者子项

    // 用于列表中的分组，例如联系人列表，新的好友列表
    QString m_groupName;    // 列表项分组名
    bool m_isOpen = false;  // 列表项是否展开
    // 分组这一条目下的子条目，例如联系人列表是一个条目，联系人列表中的每一个联系人是子条目
    // 分组列表项下的好友项，共享
    std::map<int64_t, std::shared_ptr<ContactsItem>> m_childItems;  // 列表项中的子列表项，只有列表项类型是分组项才会有
};

#endif // CONTACTSITEM_H
