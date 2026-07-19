#ifndef CONTACTSLISTWIDGET_H
#define CONTACTSLISTWIDGET_H

#include <QListWidget>
#include <vector>
#include <memory>
#include "contactsitem.h"
#include "common.h"

class ContactsListWidget : public QListWidget
{
    Q_OBJECT
signals:
    void SIG_ItemDidSelected(std::shared_ptr<ContactsItem>);

public:
    explicit ContactsListWidget(QWidget *parent = nullptr);
    ~ContactsListWidget() = default;
    ContactsListWidget(const ContactsListWidget&) = delete;
    ContactsListWidget& operator=(const ContactsListWidget&) = delete;

    void InsertItem(std::unique_ptr<ContactsItem> item);
    void UpdateItemStatus(const QString& groupname, int64_t id, FriendState status);

public slots:
    void SlotGroupOpenDidChanged();
    void SlotItemDidSelected(int64_t id, const QString& groupname);

private:
    // 刷新列表项(重绘)
    void UploadItems();
    void AddChildItem(ContactsItem* item);

signals:

private:
    // 分组列表项，共享，contactslistviewgroup要获取信息用于展示；分组列表项下的好友项，共享
    std::vector<std::shared_ptr<ContactsItem>> m_items;
    QTimer* m_updateTimer;
};

#endif // CONTACTSLISTWIDGET_H
