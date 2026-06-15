#ifndef CHATLISTITEM_H
#define CHATLISTITEM_H

#include <QWidget>
#include "friend.hpp"

namespace Ui {
class ChatListItem;
}

class ChatListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListItem(QWidget *parent = nullptr);
    ~ChatListItem();

    void SetItem(Friend *data);

private:
    Ui::ChatListItem *ui;
    Friend *m_data;
};

#endif // CHATLISTITEM_H
