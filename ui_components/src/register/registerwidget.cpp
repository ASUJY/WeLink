#include "registerwidget.h"
#include <QVBoxLayout>
#include <QFont>
#include <QDebug>
#include <QRegularExpression>

namespace {
bool IsPhoneNumberValid(const QString& phone) {
    QRegularExpression reg("^1[3-9]\\d{9}$");
    QRegularExpressionMatch match = reg.match(phone);
    return match.hasMatch();
}
}

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(420, 480);
    QVBoxLayout *vlayout = new QVBoxLayout;

    m_labInfo = new QLabel(this);
    m_labInfo->setText("欢迎注册");
    QFont font = m_labInfo->font();
    font.setPointSize(22);
    font.setBold(true);
    m_labInfo->setFont(font);

    m_leditPhone = new QLineEdit(this);
    m_labPhoneTips = new QLabel(this);
    m_leditName = new QLineEdit(this);
    m_labNameTips = new QLabel(this);
    m_leditPasswd = new QLineEdit(this);
    m_labPasswdTips = new QLabel(this);
    m_btnReg = new QPushButton(this);

    // 设置输入框高度（你要的调高）
    m_leditPhone->setFixedHeight(44);
    m_leditName->setFixedHeight(44);
    m_leditPasswd->setFixedHeight(44);

    // 设置输入框最大宽度（控件变窄）
    m_leditPhone->setFixedWidth(320);
    m_leditName->setFixedWidth(320);
    m_leditPasswd->setFixedWidth(320);

    m_btnReg->setFixedHeight(46);
    m_btnReg->setFixedWidth(320);
    m_btnReg->setObjectName("btnReg");

    m_leditPhone->setPlaceholderText("请输入手机号码");
    m_leditName->setPlaceholderText("请输入昵称");
    m_leditPasswd->setPlaceholderText("请设置密码");
    m_leditPasswd->setEchoMode(QLineEdit::Password);
    m_btnReg->setText("立即注册");


    m_rbtnPolicy = new QRadioButton(this);
    m_rbtnPolicy->setText("已阅读并同意服务协议和隐私政策");
    m_rbtnPolicy->setFixedWidth(320);
    m_rbtnPolicy->setChecked(true);

    connect(m_leditPhone, &QLineEdit::textChanged, this, &RegisterWidget::SlotCheckPhone);
    connect(m_leditName, &QLineEdit::editingFinished, this, [this](){
        if (!m_leditName->text().trimmed().isEmpty()) {
            m_labNameTips->setText("");
        }
    });
    connect(m_leditPasswd, &QLineEdit::editingFinished, this, [this](){
        if (!m_leditPasswd->text().trimmed().isEmpty()) {
            m_labPasswdTips->setText("");
        }
    });
    connect(m_btnReg, &QPushButton::clicked, this, &RegisterWidget::OnBtnRegClicked);


    // vlayout->addStretch(60);
    vlayout->addWidget(m_labInfo);
    vlayout->addSpacing(40); // 这里控制间距，40 很舒服，可自己改

    vlayout->addWidget(m_leditPhone, 0, Qt::AlignCenter);
    vlayout->addWidget(m_labPhoneTips, 0, Qt::AlignCenter);
    vlayout->addWidget(m_leditName, 0, Qt::AlignCenter);
    vlayout->addWidget(m_labNameTips, 0, Qt::AlignCenter);
    vlayout->addWidget(m_leditPasswd, 0, Qt::AlignCenter);
    vlayout->addWidget(m_labPasswdTips, 0, Qt::AlignCenter);
    vlayout->addWidget(m_btnReg, 0, Qt::AlignCenter);
    vlayout->addWidget(m_rbtnPolicy, 0, Qt::AlignCenter);
    vlayout->addStretch();

    vlayout->setSpacing(18);
    vlayout->setContentsMargins(50, 30, 50, 30);

    this->setLayout(vlayout);

}

void RegisterWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        OnBtnRegClicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void RegisterWidget::SlotCheckPhone(const QString& text) {
    if (!IsPhoneNumberValid(text)){
        m_labPhoneTips->setText("<font color='red'>请输入正确的手机号码</font>");
    } else {
        m_labPhoneTips->setText("");
    }
}

/**
 * 可以优化得更美观，可以加上校验逻辑(正则表达式)
 */
void RegisterWidget::OnBtnRegClicked() {
    QString phone = m_leditPhone->text().trimmed();
    QString username = m_leditName->text().trimmed();
    QString passwd = m_leditPasswd->text().trimmed();
    m_labPhoneTips->setText("");
    m_labNameTips->setText("");
    m_labPasswdTips->setText("");
    if (phone.isEmpty()) {
        m_labPhoneTips->setText("<font color='red'>手机号码不能为空</font>");
        m_leditPhone->setFocus();
        return;
    } else if (!IsPhoneNumberValid(phone)){
        m_labPhoneTips->setText("<font color='red'>请输入正确的手机号码</font>");
        m_leditPhone->setFocus();
        return;
    }

    if (username.isEmpty()) {
        m_labNameTips->setText("<font color='red'>用户名不能为空</font>");
        m_leditName->setFocus();
        return;
    }
    if (passwd.isEmpty()) {
        m_labPasswdTips->setText("<font color='red'>密码不能为空</font>");
        m_leditPasswd->setFocus();
        return;
    }
    emit SIG_RegisterCommit(username, phone, passwd);
}
