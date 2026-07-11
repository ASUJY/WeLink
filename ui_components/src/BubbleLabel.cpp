#include "BubbleLabel.h"
#include <QPainterPath>     // 绘制路径类，用于绘制圆角矩形+尖角的不规则气泡图形

BubbleLabel::BubbleLabel(QWidget *parent) : QLabel(parent) {
    setWordWrap(true);      // 开启文本自动换行：文本超出控件宽度时自动换行
    setMaximumWidth(230);   // 设置控件最大宽度为280像素，防止文本无限拉长

    // 设置尺寸策略：水平/垂直方向都采用「首选大小」
    // 作用：让控件根据文本内容自动适配大小，不强行拉伸/压缩
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // 设置Qt样式表：字体14px，文字颜色深灰色
    setStyleSheet("font-size:14px; color:#333;");
}

void BubbleLabel::SetBubbleType(BubbleLabel::BubbleType type) {
    m_type = type;

    // 动态设置文字边距，文字避开三角区域
    if (m_type == Receiver) {
        // 对方消息：尖角在左侧，左边距留出三角空间
        // 设置内容边距(左,上,右,下)：控件内文字和边框的间距
        setContentsMargins(m_triSize + m_paddingX, m_paddingY, m_paddingX, m_paddingY);
    } else {
        // 我方消息：尖角在右侧，右边距留出三角空间
        setContentsMargins(m_paddingX, m_paddingY, m_triSize + m_paddingX, m_paddingY);
    }

    // 触发控件重绘：立即调用 paintEvent() 刷新气泡样式
    update();
}

void BubbleLabel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿：让气泡边缘平滑，无锯齿
    QRect fullRect = rect();                        // 获取控件的客户区矩形（坐标0,0，宽高=控件实际大小）
    QPainterPath path;                              // 绘制路径：用于组合「圆角矩形+尖角」的不规则图形
    QColor bgColor;                                 // 气泡背景色

    // 获取控件宽、高、垂直中心点（尖角的位置）
    int w = fullRect.width();
    int h = fullRect.height();
    int centerY = h / 2;

    if (m_type == Sender) {
        // 我方绿色气泡，尖角朝左（右侧边缘三角）
        bgColor = QColor(143, 232, 102);            // 浅绿色背景
        // 气泡主体矩形（右侧扣除三角宽度）
        QRect bodyRect(0, 0, w - m_triSize, h);
        path.addRoundedRect(bodyRect, m_radius, m_radius);  // 添加圆角矩形：(矩形, X圆角半径, Y圆角半径)
        // 拼接右侧尖角三角形
        path.moveTo(w - m_triSize, centerY - m_triSize);    // 移动到起点
        path.lineTo(w, centerY);                            // 画第一条线
        path.lineTo(w - m_triSize, centerY + m_triSize);    // 画第二条线
        path.closeSubpath();                                // 闭合路径，形成三角形
    } else {
        // 对方白色气泡，尖角朝右（左侧边缘三角）
        bgColor = QColor(244, 244, 244);
        // 气泡主体矩形（左侧扣除三角宽度）
        QRect bodyRect(m_triSize, 0, w - m_triSize, h);
        path.addRoundedRect(bodyRect, m_radius, m_radius);
        // 拼接左侧尖角三角形
        path.moveTo(m_triSize, centerY - m_triSize);
        path.lineTo(0, centerY);
        path.lineTo(m_triSize, centerY + m_triSize);
        path.closeSubpath();
    }
    // 第一步：先绘制气泡底色（底层）
    painter.fillPath(path, bgColor);        // 用背景色填充整个绘制路径（画出气泡背景）

    // 第二步：再调用基类绘制文字（上层，不会被遮挡）
    QLabel::paintEvent(event);              // 执行 QLabel 原生的绘制逻辑：绘制文字（保证文字在背景上层）
}