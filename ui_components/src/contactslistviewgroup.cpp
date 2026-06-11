#include "contactslistviewgroup.h"
#include "ui_contactslistviewgroup.h"

ContactsListViewGroup::ContactsListViewGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsListViewGroup)
{
    ui->setupUi(this);
}

ContactsListViewGroup::~ContactsListViewGroup()
{
    delete ui;
}

void ContactsListViewGroup::SetGroupState(ContactsItem *item) {
    m_item = item;
    if (item->GetIsOpen()) {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_down.png"));
    } else {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_right.png"));
    }
    ui->labIcon->setScaledContents(true);
}


void ContactsListViewGroup::SetLabTitle(ContactsItem *item) {
    ui->labTitle->setText(item->GetGroupName());
}

void ContactsListViewGroup::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    if (m_item == nullptr) return;

    m_item->SetIsOpen(!m_item->GetIsOpen());
    if (m_item->GetIsOpen())  {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_down.png"));
    } else {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_right.png"));
    }
    // 发射信号，展开列表
}