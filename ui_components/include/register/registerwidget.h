#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QKeyEvent>

class RegisterWidget : public QWidget
{
    Q_OBJECT

signals:
    void SIG_RegisterCommit(const QByteArray& data);

private slots:
    void OnBtnRegClicked();
    void SlotCheckPhone(const QString& text);

public:
    explicit RegisterWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
   QByteArray MakeRegisterJSON();

private:
    QLabel *m_labInfo = nullptr;
    QLineEdit *m_leditPhone = nullptr;
    QLabel *m_labPhoneTips = nullptr;
    QLineEdit *m_leditName = nullptr;
    QLabel *m_labNameTips = nullptr;
    QLineEdit *m_leditPasswd = nullptr;
    QLabel *m_labPasswdTips = nullptr;
    QPushButton* m_btnReg = nullptr;
    QRadioButton *m_rbtnPolicy = nullptr;

};

#endif // REGISTERWIDGET_H
