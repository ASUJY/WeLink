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

void ChatListItem::SetItem(std::unique_ptr<Friend>&& data) {
    if (data == nullptr) return;

    m_data = std::move(data);
    auto pixmap = QPixmap(":/resource/head/man.svg");
    // 设置图片自动缩放
    ui->labHeadIcon->setPixmap(pixmap.scaled(ui->labHeadIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->labHeadIcon->setScaledContents(true);
    ui->labFriName->setText(QString::fromStdString(m_data->GetUserName()));
    // ui->labFriName->setStyleSheet("color: black;");
    ui->labTime->setText(m_data->GetTime());
    if (m_data->GetMessages().count() > 0) {
        // setContent(data)
    }

    int unreadCnt = m_data->GetCount();
    ui->labMsgCount->setText(QString::number(unreadCnt));
    ui->chatlistitem_widget->setVisible(unreadCnt != 0);
    ui->labMute->setVisible(m_data->GetMute());

    // 存在消息则刷新最新预览文本
    const auto& msgList = m_data->GetMessages();
    if (!msgList.empty())
    {
        SetContent(msgList.back().GetContent());
    }
}

void ChatListItem::Checked(bool checked) {
    m_isChecked = checked;
}

void ChatListItem::Selected(bool selected) {
    if (selected) {
        ui->widget->setStyleSheet("QWidget#widget{background: #2cb46e;}");
    } else {
        ui->widget->setStyleSheet("QWidget#widget{background: #f4f4f4;}");
    }
}

void ChatListItem::mousePressEvent(QMouseEvent *event) {
    // 点击清空未读计数、隐藏红点
    ui->labMsgCount->setText("0");
    ui->chatlistitem_widget->setVisible(false);
    emit SIG_Selected(this);

    // 保留基类鼠标事件分发，防止点击事件失效、拖拽/选中异常
    QWidget::mousePressEvent(event);
}

void ChatListItem::UpdateFriend(const Message& message) {
    if (!m_data)
        return;

    m_data->AddMessage(message);
}

void ChatListItem::SetContent(const QString& content)
{
    QFontMetrics metrics(font()); // 字体度量对象，用于测量文本的宽度。
    // 根据给定的最大宽度(150像素)对文本进行截断，并添加省略号（...）以表示被截断了。截断的位置是在文本的右侧
    auto new_content = metrics.elidedText(content, Qt::ElideRight, 150);
    ui->labMsg->setText(new_content);
}

void ChatListItem::UpdateContent(const Message& messages) {
    qDebug() << "ChatListItem::UpdateContent: message content:" << messages.GetContent()
             << " message type: " << messages.GetMsgType() << " msg time: " << messages.GetTime();
    if (!m_data)
        return;

    m_data->AddMessage(messages);
    m_data->AddCount(1);
    ui->labMsgCount->setText(QString::number(m_data->GetCount()));
    ui->chatlistitem_widget->setVisible(m_data->GetCount() != 0);

    const QString& newTime = messages.GetTime();

    m_data->SetTime(newTime);
    ui->labTime->setText(newTime);

    const auto& msgList = m_data->GetMessages();
    if (msgList.count() > 0) {
        SetContent(msgList.last().GetContent());
    }
}

void ChatListItem::UpdateContent2(const Message& messages) {
    qDebug() << "ChatListItem::UpdateContent: message content:" << messages.GetContent()
    << " message type: " << messages.GetMsgType() << " msg time: " << messages.GetTime();
    if (!m_data)
        return;

    m_data->AddMessage(messages);
    emit SIG_Selected(this);
}

QSize ChatListItem::sizeHint() const
{
    return QSize(250, 70); // 返回自定义的尺寸
}

void ChatListItem::enterEvent(QEnterEvent *event)
{
    if (m_isChecked) return;
    ui->widget->setStyleSheet("background-color: #c3c3c3;");
    QWidget::enterEvent(event);
}

void ChatListItem::leaveEvent(QEvent *event)
{
    // 离开时恢复选中态或普通态
    ui->widget->setStyleSheet(
        m_isChecked
            ? "background-color: #2cb46e;"
            : "background-color: #f4f4f4;"
        );
    QWidget::leaveEvent(event);
}