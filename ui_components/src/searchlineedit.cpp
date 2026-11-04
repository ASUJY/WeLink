#include "searchlineedit.h"

namespace {
constexpr int len = 128;
}

SearchLineEdit::SearchLineEdit(QWidget* parent) : QLineEdit(parent), m_maxLen(len) {
    connect(this, &QLineEdit::textChanged, this, &SearchLineEdit::LimitTextLength);
}

void SearchLineEdit::SetMaxLength(int len) {
    m_maxLen = len;
}

void SearchLineEdit::LimitTextLength(QString text) {
    if (m_maxLen <= 0) {
        return;
    }

    QByteArray byteArray = text.toUtf8();
    if (byteArray.size() > m_maxLen) {
        byteArray = byteArray.left(m_maxLen);
        this->setText(QString::fromUtf8(byteArray));
    }
}

void SearchLineEdit::focusOutEvent(QFocusEvent *event) {
    // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    //发送失去焦点得信号
    emit SIG_FOUCUS_OUT();
}