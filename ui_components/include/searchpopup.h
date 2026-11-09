#ifndef SEARCHPOPUP_H
#define SEARCHPOPUP_H

#include <QFrame>

class SearchPopup : public QFrame
{
    Q_OBJECT
public:
    SearchPopup(QWidget* parent = nullptr);
    ~SearchPopup();

    // 弹窗显示在指定控件下方
    void ShowBelow(QWidget* widget);
    void HidePopup();
};

#endif // SEARCHPOPUP_H
