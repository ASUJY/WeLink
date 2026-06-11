#include "chatmainwidget.h"
#include "ui_chatmainwidget.h"

ChatMainWidget::ChatMainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatMainWidget)
{
    ui->setupUi(this);
}

ChatMainWidget::~ChatMainWidget()
{
    delete ui;
}
