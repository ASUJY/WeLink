#include "senderwidget.h"
#include "ui_senderwidget.h"
#include "label.h"

SenderWidget::SenderWidget(QWidget *parent,  QString imageUrl)
    : QWidget(parent)
    , ui(new Ui::SenderWidget)
{
    ui->setupUi(this);
    Init(imageUrl);
}

SenderWidget::~SenderWidget()
{
    delete ui;
}

void SenderWidget::Init(QString imageUrl) {
    ui->btnHead->setIcon(QIcon(imageUrl));
    connect(ui->labMsg, &Label::SIG_LabelSizeChanged, this, &SenderWidget::SlotLabelSizeChanged);
}

void SenderWidget::SetMessage(QString message) {
    ui->labMsg->SetText(message);
}

void SenderWidget::SlotLabelSizeChanged(QRect rect) {
    auto rect1 = geometry();
    rect1.setHeight(rect.height() + 20);
    setGeometry(rect1);
    emit SIG_LabelSizeChanged(rect);
}