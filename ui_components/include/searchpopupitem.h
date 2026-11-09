#ifndef SEARCHPOPUPITEM_H
#define SEARCHPOPUPITEM_H

#include <QWidget>

namespace Ui {
class SearchPopupItem;
}

class SearchPopupItem : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPopupItem(QWidget *parent = nullptr);
    ~SearchPopupItem();

    void SetText(const QString& text);

private:
    Ui::SearchPopupItem *ui;
};

#endif // SEARCHPOPUPITEM_H
