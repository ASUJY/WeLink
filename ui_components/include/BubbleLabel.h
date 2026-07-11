#ifndef BUBBLELABEL_H
#define BUBBLELABEL_H

#include <QLabel>
#include <QPainter>

// 自定义气泡标签类
class BubbleLabel : public QLabel {
    Q_OBJECT
public:
    enum BubbleType {
        Receiver = 0,
        Sender
    };

    explicit BubbleLabel(QWidget *parent = nullptr);
    void SetBubbleType(BubbleType type);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    BubbleType m_type = Receiver;
    const int m_radius = 8;     // 气泡圆角大小
    const int m_triSize = 6;    // 尖角三角尺寸
    const int m_paddingX = 12;  // 文字左右内边距
    const int m_paddingY = 8;   // 文字上下内边距
};

#endif // BUBBLELABEL_H