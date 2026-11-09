#ifndef SEARCHPOPUP_H
#define SEARCHPOPUP_H

#include <QFrame>
#include <QListWidget>
#include "searchpopupitem.h"

class SearchPopup : public QFrame
{
    Q_OBJECT

signals:
    void SIG_AddFriendClicked(const QString text);

public:
    SearchPopup(QWidget* parent = nullptr);
    ~SearchPopup();

    // 弹窗显示在指定控件下方
    void ShowBelow(QWidget* widget);
    void HidePopup();

    void SetItemText(const QString& text);
private:
    QListWidget* m_listWidget = nullptr;
    SearchPopupItem* m_searchPopupItem = nullptr;
};

#endif // SEARCHPOPUP_H
