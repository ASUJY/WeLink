#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
signals:
    void SIG_FOUCUS_OUT();
public:
    explicit SearchLineEdit(QWidget* parent = nullptr);
    ~SearchLineEdit() = default;
    void SetMaxLength(int maxLen);

protected:
    void focusOutEvent(QFocusEvent *event) override;

private:
    void LimitTextLength(QString text);

private:
    int m_maxLen = 0;
};

#endif // SEARCHLINEEDIT_H
