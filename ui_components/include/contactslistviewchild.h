#ifndef CONTACTSLISTVIEWCHILD_H
#define CONTACTSLISTVIEWCHILD_H

#include <QWidget>
#include <QMouseEvent>

class ContactsListViewChild : public QWidget
{
    Q_OBJECT

signals:
    void SIG_ItemDidSelected(int64_t id, const QString& groupname);

public:
    explicit ContactsListViewChild(int64_t id,
                                   const QString& name,
                                   const QString& headIcon,
                                   const QString& groupname,
                                   QWidget* parent = nullptr) noexcept;
    ~ContactsListViewChild();

    // void SetItem(ContactsItem* item) {m_item = item;}
    // ContactsItem* GetItem() {return m_item;}

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

private:
    int64_t m_userid;
    QString m_username;
    QString m_headIcon;
    QString m_groupname;
//     ContactsItem *m_item = nullptr;
};

#endif // CONTACTSLISTVIEWCHILD_H
