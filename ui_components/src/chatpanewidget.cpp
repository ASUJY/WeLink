#include "chatpanewidget.h"
#include "ui_chatpanewidget.h"
#include <QToolButton>
#include <QMenu>
#include <QDebug>

ChatPaneWidget::ChatPaneWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPaneWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    ui->btnPlus->setPopupMode(QToolButton::InstantPopup);
    ui->btnPlus->setStyleSheet("QToolButton:menu-indicator {image: none;}");

    QMenu *menu = new QMenu(this);
    QAction *action1 = menu->addAction("添加朋友");
    menu->addAction("选项2");
    connect(action1, &QAction::triggered, this, [=](){
        qDebug() << "添加朋友";
        emit SIG_AddFriend();
    });
    ui->btnPlus->setMenu(menu);



}

ChatPaneWidget::~ChatPaneWidget()
{
    delete ui;
}
