#ifndef CONTACTSITEM_H
#define CONTACTSITEM_H

#include <QString>
#include <QList>

enum ContactsItemType : int {
    Group = 0,
    Item,
};

enum ContactsState : int {
    Done = 0,
    Send,
    Recevie
};

class ContactsItem
{
public:
    ContactsItem() = default;
    ~ContactsItem() {
        int n = m_childItems.size();
        if (n > 0) {
            for (ContactsItem* item : m_childItems) {
                delete item;
            }
            m_childItems.clear();
        }
    }

    void SetGroupName(QString name) {m_groupName = name;}
    void SetIsOpen(bool isopen) {m_isOpen = isopen;}
    void SetItemName(QString name) {m_name = name;}
    void SetHeadIcon(QString headIcon) {m_headIcon = headIcon;}
    void SetItemType(int type) {m_type = type;}
     void SetItemState(int state) {m_state = state;}
    void AddChildItem(ContactsItem* item) {
        if (item) m_childItems.append(item);
    }
    QString GetGroupName() {return m_groupName;}
    bool GetIsOpen() {return m_isOpen;}
    QString GetItemName() {return m_name;}
    int GetItemType() {return m_type;}
    int GetItemState() {return m_state;}
    QString GetHeadIcon() {return m_headIcon;}
    QList<ContactsItem *> GetChildItems() {return m_childItems;}

private:
    QString m_name;
    QString m_headIcon;
    int m_state = Done;
    int m_type = ContactsItemType::Group;

    // 用于列表中的分组，例如联系人列表，新的好友列表
    QString m_groupName;
    bool m_isOpen = false;

    // 分组这一条目下的子条目，例如联系人列表是一个条目，联系人列表中的每一个联系人是子条目
    QList<ContactsItem *> m_childItems;
};

#endif // CONTACTSITEM_H
