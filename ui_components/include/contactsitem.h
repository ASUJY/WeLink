#ifndef CONTACTSITEM_H
#define CONTACTSITEM_H

#include <QString>

enum ContactsItemType : int {
    Group = 0,
};

class ContactsItem
{
public:
    ContactsItem() = default;

    void SetGroupName(QString name) {m_groupName = name;}
    void SetIsOpen(bool isopen) {m_isOpen = isopen;}
    void SetItemName(QString name) {m_name = name;}
    void SetItemType(int type) {m_type = type;}
    QString GetGroupName() {return m_groupName;}
    bool GetIsOpen() {return m_isOpen;}
    QString GetItemName() {return m_name;}
    int GetItemType() {return m_type;}

private:
    QString m_name;
    int m_type = ContactsItemType::Group;

    // 用于列表中的分组，例如联系人列表，新的好友列表
    QString m_groupName;
    bool m_isOpen = false;
};

#endif // CONTACTSITEM_H
