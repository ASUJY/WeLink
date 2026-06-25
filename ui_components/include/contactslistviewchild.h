#ifndef CONTACTSLISTVIEWCHILD_H
#define CONTACTSLISTVIEWCHILD_H

#include <QWidget>
#include <QMouseEvent>

class ContactsListViewChild : public QWidget
{
    Q_OBJECT

signals:
    void SIG_ItemDidSelected(uint64_t id);

public:
    explicit ContactsListViewChild(uint64_t id, const QString& name, const QString& headIcon, QWidget* parent = nullptr);
    ~ContactsListViewChild();

    // void SetItem(ContactsItem* item) {m_item = item;}
    // ContactsItem* GetItem() {return m_item;}

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    uint64_t m_userid;
    QString m_username;
    QString m_headIcon;
//     ContactsItem *m_item = nullptr;
};

#endif // CONTACTSLISTVIEWCHILD_H
