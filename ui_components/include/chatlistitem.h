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

signals:
    void SIG_Selected(ChatListItem *item);

public:
    explicit ChatListItem(QWidget *parent = nullptr);
    ~ChatListItem();

    void Checked(bool checked);
    void Selected(bool selected);

    void SetItem(Friend *data);
    Friend *GetItem() const { return m_data; }

private:
    // 鼠标按下聊天列表项触发这个函数
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::ChatListItem *ui;
    Friend *m_data;
    bool m_isChecked;   // 该item是否被点击
};

#endif // CHATLISTITEM_H
