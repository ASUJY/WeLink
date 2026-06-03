#include "registerwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QFont>

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(420, 480);
    QVBoxLayout *vlayout = new QVBoxLayout;

    QLabel *labInfo = new QLabel(this);
    labInfo->setText("欢迎注册");
    QFont font = labInfo->font();
    font.setPointSize(22);
    font.setBold(true);
    labInfo->setFont(font);

    QLineEdit *leditPhone = new QLineEdit(this);
    QLineEdit *leditName = new QLineEdit(this);
    QLineEdit *leditPasswd = new QLineEdit(this);
    QPushButton *btnReg = new QPushButton(this);

    // 设置输入框高度（你要的调高）
    leditPhone->setFixedHeight(44);
    leditName->setFixedHeight(44);
    leditPasswd->setFixedHeight(44);

    // 设置输入框最大宽度（控件变窄）
    leditPhone->setFixedWidth(320);
    leditName->setFixedWidth(320);
    leditPasswd->setFixedWidth(320);

    btnReg->setFixedHeight(46);
    btnReg->setFixedWidth(320);
    btnReg->setObjectName("btnReg");

    leditPhone->setPlaceholderText("请输入手机号码");
    leditName->setPlaceholderText("请输入昵称");
    leditPasswd->setPlaceholderText("请设置密码");
    leditPasswd->setEchoMode(QLineEdit::Password);
    btnReg->setText("立即注册");


    QRadioButton *rbtnPolicy = new QRadioButton(this);
    rbtnPolicy->setText("已阅读并同意服务协议和隐私政策");
    rbtnPolicy->setFixedWidth(320);

    // vlayout->addStretch(60);
    vlayout->addWidget(labInfo);
    vlayout->addSpacing(40); // 这里控制间距，40 很舒服，可自己改

    vlayout->addWidget(leditPhone, 0, Qt::AlignCenter);
    vlayout->addWidget(leditName, 0, Qt::AlignCenter);
    vlayout->addWidget(leditPasswd, 0, Qt::AlignCenter);
    vlayout->addWidget(btnReg, 0, Qt::AlignCenter);
    vlayout->addWidget(rbtnPolicy, 0, Qt::AlignCenter);
    vlayout->addStretch();

    vlayout->setSpacing(18);
    vlayout->setContentsMargins(50, 30, 50, 30);

    this->setLayout(vlayout);

}
