#ifndef RECEIVERWIDGET_H
#define RECEIVERWIDGET_H

#include <QWidget>

namespace Ui {
class ReceiverWidget;
}

class ReceiverWidget : public QWidget
{
    Q_OBJECT
signals:
    void SIG_LabelSizeChanged(QRect rect);

public:
    explicit ReceiverWidget(QWidget *parent = nullptr, QString imageUrl = "");
    ~ReceiverWidget();

    void SetMessage(QString message);

private:
    void Init(QString imageUrl);
    void SlotLabelSizeChanged(QRect rect);

private:
    Ui::ReceiverWidget *ui;
};

#endif // RECEIVERWIDGET_H
