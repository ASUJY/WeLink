#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "clicklabel.h"
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QStyle>
#include <QIcon>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    /* 加载样式 */
    QFile file(":/qss/resource/qss/default.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    this->setWindowTitle("");
    this->setFixedSize(320, 460);
    this->setWindowIcon(QIcon(":/resource/icon/app.png")); // 设置默认图标

    // 无边框窗口(去掉系统自带标题栏)
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    // 背景透明
    // this->setAttribute(Qt::WA_TranslucentBackground);

    SetWidgetWinTitle();
    SetWidgetHeadIcon();
    SetWidgetWinBody();
    SetWidgetWinBottom();

}

void LoginWidget::SetWidgetWinTitle() {
    ui->widgetWinTitle->setFixedHeight(24);
    QSize btnSize(12, 12);

    ui->btnWinClose->setFixedSize(btnSize);
    ui->btnWinMin->setFixedSize(btnSize);
    ui->btnWinMax->setFixedSize(btnSize);

    ui->btnWinClose->setFlat(true);
    ui->btnWinMin->setFlat(true);
    ui->btnWinMax->setFlat(true);
}

void LoginWidget::SetWidgetHeadIcon() {
    ui->widgetHeadIcon->setFixedHeight(120);
    ui->labelUserHeadIcon->setFixedWidth(120);
    ui->labelUserHeadIcon->setPixmap(QPixmap(":/resource/icon/app.png"));
    ui->labelUserHeadIcon->setScaledContents(true);
}

void LoginWidget::SetWidgetWinBody() {
    // 输入框默认提示文字
    ui->lineEditUser->setPlaceholderText("QQ号码/手机/邮箱");
    ui->lineEditPwd->setPlaceholderText("请输入密码");
    ui->lineEditPwd->setEchoMode(QLineEdit::Password); // 密码隐藏

    ui->lineEditUser->setFixedSize(260, 40);
    ui->lineEditPwd->setFixedSize(260, 40);
    ui->btnLogin->setFixedSize(260, 40);
}

void LoginWidget::SetWidgetWinBottom() {
    QHBoxLayout *layout = new QHBoxLayout;

    // ClickLabel *labScan = new ClickLabel(ui->widgetWinBottom);
    // labScan->setText("扫码登录");

    // 分隔线
    // QFrame *line = new QFrame(ui->widgetWinBottom);
    // line->setFrameShape(QFrame::VLine);
    // line->setFrameShadow(QFrame::Plain);
    // line->setStyleSheet("border:none; background-color:#cccccc;");
    // line->setFixedSize(1, 14);

    ClickLabel *labReg = new ClickLabel(ui->widgetWinBottom);
    labReg->setText("注册帐号");

    connect(labReg, &ClickLabel::clicked, this, [](){

    });


    // layout->addWidget(labScan);
    // layout->addWidget(line);
    layout->addWidget(labReg);
    layout->setAlignment(Qt::AlignCenter);
    ui->widgetWinBottom->setLayout(layout);

}

LoginWidget::~LoginWidget()
{
    delete ui;
}
