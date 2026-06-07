#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    SetWidgetWinTitle();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::SetWidgetWinTitle() {
    ui->winTitleWidget->setFixedHeight(24);

    QSize btnSize(12,12);
    ui->btnMWinClose->setFixedSize(btnSize);
    ui->btnMWinMin->setFixedSize(btnSize);
    ui->btnMWinMax->setFixedSize(btnSize);

    ui->btnMWinClose->setFlat(true);
    ui->btnMWinMin->setFlat(true);
    ui->btnMWinMax->setFlat(true);

    connect(ui->btnMWinClose, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->btnMWinMin, &QPushButton::clicked, this, [=](){
        this->showMinimized();
    });
    connect(ui->btnMWinMax, &QPushButton::clicked, this, [=](){
        this->showMaximized();
    });
}