#include "contactslistviewgroup.h"
#include "ui_contactslistviewgroup.h"

const QString ContactsListViewGroup::ICON_ARROW_DOWN  = ":/resource/icon/menu/aio_arrow_down.png";
const QString ContactsListViewGroup::ICON_ARROW_RIGHT = ":/resource/icon/menu/aio_arrow_right.png";

ContactsListViewGroup::ContactsListViewGroup(const std::shared_ptr<ContactsItem>& item, QWidget *parent) noexcept
    : QWidget(parent)
    , ui(new Ui::ContactsListViewGroup),m_item(item)
{
    ui->setupUi(this);
    UpdateGroupVisual();
    UpdateTitleText();
}

ContactsListViewGroup::~ContactsListViewGroup()
{
    delete ui;
}

void ContactsListViewGroup::UpdateGroupVisual() {
    if (!m_item)
        return;

    if (m_item->GetIsOpen()) {
        ui->labIcon->setPixmap(QPixmap(ICON_ARROW_DOWN));
    } else {
        ui->labIcon->setPixmap(QPixmap(ICON_ARROW_RIGHT));
    }
    ui->labIcon->setScaledContents(true);
}


void ContactsListViewGroup::UpdateTitleText() {
    if (!m_item)
        return;

    ui->labTitle->setText(m_item->GetGroupName());
    ui->labTitle->setStyleSheet("color: black;");
}

void ContactsListViewGroup::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        // 调用基类，保证其他鼠标事件正常分发
        QWidget::mousePressEvent(event);
        return;
    }
    if (m_item == nullptr) return;

    m_item->SetIsOpen(!m_item->GetIsOpen());
    UpdateGroupVisual();
    // 发射信号，展开列表
    emit SIG_GroupOpenStatusDidChanged();
}