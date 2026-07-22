#ifndef CONTACTSLISTVIEWGROUP_H
#define CONTACTSLISTVIEWGROUP_H

#include <QWidget>
#include <QMouseEvent>
#include "contactsitem.h"

namespace Ui {
class ContactsListViewGroup;
}

class ContactsListViewGroup : public QWidget
{
    Q_OBJECT

signals:
    void SIG_GroupOpenStatusDidChanged();

public:
    explicit ContactsListViewGroup(const std::shared_ptr<ContactsItem>& item, QWidget *parent = nullptr) noexcept;
    ~ContactsListViewGroup();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void UpdateGroupVisual();
    void UpdateTitleText();

private:
    Ui::ContactsListViewGroup *ui;
    std::shared_ptr<ContactsItem> m_item;

    static const QString ICON_ARROW_DOWN;
    static const QString ICON_ARROW_RIGHT;

};

#endif // CONTACTSLISTVIEWGROUP_H
