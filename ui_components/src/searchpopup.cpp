#include "searchpopup.h"
#include <QApplication>
#include <QMouseEvent>
#include <QLayout>

SearchPopup::SearchPopup(QWidget* parent) : QFrame(parent, Qt::Tool | Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(250, 200);
    setObjectName("SearchPopup");
    setStyleSheet(
        "QFrame#SearchPopup  {"
        "    background-color: white;"
        "    border: 1px solid #DDDDDD;"
        "    border-radius: 6px;"
        "}"
        );

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_listWidget = new QListWidget(this);
    m_listWidget->setFrameShape(QFrame::NoFrame);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_searchPopupItem = new SearchPopupItem(m_listWidget);

    auto* listWidItem = new QListWidgetItem(m_listWidget);
    m_listWidget->addItem(listWidItem);
    m_listWidget->setItemWidget(listWidItem, m_searchPopupItem);
    listWidItem->setSizeHint(QSize(250, 45));
    layout->addWidget(m_listWidget);

    connect(m_listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* clickedItem) {
        if (m_listWidget->itemWidget(clickedItem) == m_searchPopupItem) {
            HidePopup();
            emit SIG_AddFriendClicked(m_searchPopupItem->Text());
        }
    });
}

SearchPopup::~SearchPopup()
{
}

void SearchPopup::ShowBelow(QWidget *widget) {
    if (!widget) {
        return;
    }

    // 获取控件左下角的全局坐标
    QPoint pos = widget->mapToGlobal(QPoint(0, widget->height()));

    this->move(pos);
    this->show();
}

void SearchPopup::HidePopup() {
    this->hide();
}

void SearchPopup::SetItemText(const QString& text) {
    if (m_searchPopupItem) {
        m_searchPopupItem->SetText(text);
    }
}