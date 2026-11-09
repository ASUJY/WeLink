#include "searchpopupitem.h"
#include "ui_searchpopupitem.h"

SearchPopupItem::SearchPopupItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchPopupItem)
{
    ui->setupUi(this);
    ui->labPopItemIcon->setPixmap(QPixmap(":/resource/icon/menu/add_friend.png"));
}

SearchPopupItem::~SearchPopupItem()
{
    delete ui;
}

void SearchPopupItem::SetText(const QString &text)
{
    ui->labPopItemText->setText(text);
}
