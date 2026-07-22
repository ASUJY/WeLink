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

    explicit BubbleLabel(QWidget *parent = nullptr) noexcept;
    ~BubbleLabel() = default;
    void SetBubbleType(BubbleType type);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    BubbleType m_type = Receiver;
    static const int BUBBLE_RADIUS;
    static const int TRIANGLE_SIZE;
    static const int PADDING_X;
    static const int PADDING_Y;
    static const int MAX_WIDTH;
};

#endif // BUBBLELABEL_H