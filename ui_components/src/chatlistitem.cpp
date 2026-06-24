#include "chatlistitem.h"
#include "ui_chatlistitem.h"

ChatListItem::ChatListItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatListItem)
{
    ui->setupUi(this);
}

ChatListItem::~ChatListItem()
{
    delete ui;
}

void ChatListItem::SetItem(std::unique_ptr<Friend> data) {
    if (data == nullptr) return;

    m_data = std::move(data);
    auto pixmap = QPixmap(":/resource/head/man.svg");
    ui->labHeadIcon->setPixmap(pixmap);
    ui->labName->setText(QString::fromStdString(m_data->GetUserName()));
    ui->labName->setStyleSheet("color: black;");
    ui->labTime->setText(m_data->GetTime());
    if (m_data->GetMessages().count() > 0) {
        // setContent(data)
    }
    ui->labMsgCount->setText(QString::number(m_data->GetCount()));
    ui->chatlistitem_widget->setVisible(m_data->GetCount() != 0);
    ui->labMute->setVisible(m_data->GetMute());
}

void ChatListItem::Checked(bool checked) {
    m_isChecked = checked;
}

void ChatListItem::Selected(bool selected) {
    if (selected) {
        ui->widget->setStyleSheet("QWidget#widget{background: #c3c3c3;}");
    } else {
        ui->widget->setStyleSheet("QWidget#widget{background: #dfdfdf;}");
    }
}

void ChatListItem::mousePressEvent(QMouseEvent *event) {
    ui->labMsgCount->setText("0");
    ui->chatlistitem_widget->setVisible(false);
    emit SIG_Selected(this);
}

void ChatListItem::UpdateFriend(Message& message) {
    m_data->AddMessage(message);
}

void ChatListItem::SetContent(QString content)
{
    QFontMetrics metrics(font()); // 字体度量对象，用于测量文本的宽度。
    // 根据给定的最大宽度(150像素)对文本进行截断，并添加省略号（...）以表示被截断了。截断的位置是在文本的右侧
    auto new_content = metrics.elidedText(content, Qt::ElideRight, 150);
    ui->labMsg->setText(new_content);
}

void ChatListItem::UpdateContent(Message& messages) {
    qDebug() << "ChatListItem::UpdateContent: message content:" << messages.GetContent()
             << " message type: " << messages.GetMsgType() << " msg time: " << messages.GetTime();
    m_data->AddMessage(messages);
    m_data->AddCount(1);
    ui->labMsgCount->setText(QString::number(m_data->GetCount()));
    ui->chatlistitem_widget->setVisible(m_data->GetCount() != 0);
    m_data->SetTime(messages.GetTime());
    if (m_data->GetMessages().count() > 0) {
        SetContent(m_data->GetMessages().last().GetContent());
    }
    ui->labTime->setText(m_data->GetTime());
}