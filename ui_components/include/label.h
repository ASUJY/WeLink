
#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = nullptr);
    void SetText(QString text);

signals:
    void SIG_LabelSizeChanged(QRect size);
};

#endif // LABEL_H
