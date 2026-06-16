#ifndef SENDERWIDGET_H
#define SENDERWIDGET_H

#include <QWidget>
#include <QObject>

namespace Ui {
class SenderWidget;
}

class SenderWidget : public QWidget
{
    Q_OBJECT

signals:
    void SIG_LabelSizeChanged(QRect rect);

public:
    explicit SenderWidget(QWidget *parent = nullptr, QString imageUrl = "");
    ~SenderWidget();

    void SetMessage(QString message);

private:
    void Init(QString imageUrl);
    void SlotLabelSizeChanged(QRect rect);

private:
    Ui::SenderWidget *ui;
};

#endif // SENDERWIDGET_H
