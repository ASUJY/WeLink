#ifndef CONTACTSITEM_H
#define CONTACTSITEM_H

#include <QString>
#include <QMap>
#include <memory>
#include "common.h"

enum ContactsItemType : int {
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
    void SetItemId(uint64_t id) {m_id = id;}
    void SetHeadIcon(const QString& headIcon) {m_headIcon = headIcon;}
    void SetItemType(int type) {m_type = type;}
    void SetItemState(FriendState state) {m_state = state;}
    void AddChildItem(uint64_t id, std::unique_ptr<ContactsItem> item) {
        m_childItems[id] = std::move(item);
    }
    QString GetGroupName() const {return m_groupName;}
    bool GetIsOpen() const {return m_isOpen;}
    QString GetItemName() const {return m_name;}
    uint64_t GetItemId() const {return m_id;}
    int GetItemType() const {return m_type;}
    FriendState GetItemState() const {return m_state;}
    QString GetHeadIcon() const {return m_headIcon;}
    const std::map<uint64_t, std::shared_ptr<ContactsItem>>& GetChildItems() const {return m_childItems;}
    std::shared_ptr<ContactsItem> GetChildItemById(uint64_t id) const {
        auto it = m_childItems.find(id);
        if (it != m_childItems.end()) {
            return it->second;  // 拷贝shared_ptr，引用计数+1，调用方持有副本
        }
        return nullptr;
    }

private:
    QString m_name;
    uint64_t m_id;
    QString m_headIcon;
    FriendState m_state = FriendState::DONE;
    int m_type = ContactsItemType::Group;

    // 用于列表中的分组，例如联系人列表，新的好友列表
    QString m_groupName;
    bool m_isOpen = false;
    // 分组这一条目下的子条目，例如联系人列表是一个条目，联系人列表中的每一个联系人是子条目
    // 分组列表项下的好友项，共享
    std::map<uint64_t, std::shared_ptr<ContactsItem>> m_childItems;
};

#endif // CONTACTSITEM_H
