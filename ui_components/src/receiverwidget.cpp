#include "receiverwidget.h"
#include "ui_receiverwidget.h"

ReceiverWidget::ReceiverWidget(QWidget *parent, QString imageUrl)
    : QWidget(parent)
    , ui(new Ui::ReceiverWidget)
{
    ui->setupUi(this);
    Init(imageUrl);
}

ReceiverWidget::~ReceiverWidget()
{
    delete ui;
}

void ReceiverWidget::Init(QString imageUrl) {
    ui->btnHead->setIcon(QIcon(imageUrl));
    connect(ui->labMsg, &Label::SIG_LabelSizeChanged, this, &ReceiverWidget::SlotLabelSizeChanged);
}

void ReceiverWidget::SetMessage(QString message) {
    ui->labMsg->setStyleSheet("color: black;");
    ui->labMsg->SetText(message);
}

void ReceiverWidget::SlotLabelSizeChanged(QRect rect) {
    auto rect1 = geometry();
    rect1.setHeight(rect.height() + 20);
    setGeometry(rect1);
    emit SIG_LabelSizeChanged(rect);
}