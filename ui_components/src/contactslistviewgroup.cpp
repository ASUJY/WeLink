#include "contactslistviewgroup.h"
#include "ui_contactslistviewgroup.h"

ContactsListViewGroup::ContactsListViewGroup(const std::shared_ptr<ContactsItem>& item, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactsListViewGroup)
{
    ui->setupUi(this);
    m_item = item;
    SetGroupState();
    SetLabTitle();
}

ContactsListViewGroup::~ContactsListViewGroup()
{
    delete ui;
}

void ContactsListViewGroup::SetGroupState() {
    if (m_item->GetIsOpen()) {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_down.png"));
    } else {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_right.png"));
    }
    ui->labIcon->setScaledContents(true);
}


void ContactsListViewGroup::SetLabTitle() {
    ui->labTitle->setText(m_item->GetGroupName());
    ui->labTitle->setStyleSheet("color: black;");
}

void ContactsListViewGroup::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) return;
    if (m_item == nullptr) return;

    m_item->SetIsOpen(!m_item->GetIsOpen());
    if (m_item->GetIsOpen())  {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_down.png"));
    } else {
        ui->labIcon->setPixmap(QPixmap(":/resource/icon/menu/aio_arrow_right.png"));
    }
    // 发射信号，展开列表
    emit SIG_GroupOpenStatusDidChanged();
}