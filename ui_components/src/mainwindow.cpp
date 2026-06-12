#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_page(ChatWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    SetWidgetWinTitle();
    m_addFriendWindow = new AddFriendWindow;
    connect(m_addFriendWindow, &AddFriendWindow::SIG_GetFriendInfo, this, &MainWindow::SIG_GetFriendInfo);
    connect(this, &MainWindow::SIG_GetFriendInfoSuccess, m_addFriendWindow, &AddFriendWindow::SlotGetFriendInfoSuccess);
    connect(m_addFriendWindow, &AddFriendWindow::SIG_AddFriendReq, this, &MainWindow::SIG_AddFriendReq);

    m_chatPaneWidget = new ChatPaneWidget;
    connect(m_chatPaneWidget, &ChatPaneWidget::SIG_AddFriend, this, &MainWindow::ShowAddFriendWindow);
    ui->gridLayout_2->addWidget(m_chatPaneWidget);
    m_chatPaneWidget->show();
    m_chatMainWidget = new ChatMainWidget;


    m_contactsPaneWidget = new ContactsPaneWidget;
    connect(this, &MainWindow::SIG_ReciveAddFriendReq, m_contactsPaneWidget, &ContactsPaneWidget::SlotReciveAddFriendReq);

    // this->setMouseTracking(true);
    // this->installEventFilter(this); // 将事件过滤器对象(this)安装到目标对象(this)上

    m_btn = ui->btnAllChat;
    connect(ui->btnAllChat, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
    connect(ui->btnContact, &QPushButton::clicked, this, &MainWindow::SlotSelectEvent);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::SetWidgetWinTitle() {
    ui->winTitleWidget->setFixedHeight(24);

    QSize btnSize(12,12);
    ui->btnMWinClose->setFixedSize(btnSize);
    ui->btnMWinMin->setFixedSize(btnSize);
    ui->btnMWinMax->setFixedSize(btnSize);

    ui->btnMWinClose->setFlat(true);
    ui->btnMWinMin->setFlat(true);
    ui->btnMWinMax->setFlat(true);

    connect(ui->btnMWinClose, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->btnMWinMin, &QPushButton::clicked, this, [=](){
        this->showMinimized();
    });
    connect(ui->btnMWinMax, &QPushButton::clicked, this, [=](){
        this->showMaximized();
    });
}

/**
 * 根据鼠标坐标(x,y)判断鼠标位于窗口的哪个区域
 */
Area MainWindow::GetArea(int x, int y) {
    auto rect = geometry();             // 获取当前窗口的几何信息(位置和尺寸)
    constexpr int resizemargin = 10;    // 距离窗口边缘10像素内视为边缘区域
    if (x <= resizemargin) {            // 左侧边缘
        if (y >= rect.height() - resizemargin) {
            return Area::BottomLeft;
        } else if (y <= resizemargin) {
            return Area::TopLeft;
        } else {
            return Area::Left;
        }
    } else if (x >= rect.width() - resizemargin) {  // 右侧边缘
        if (y >= rect.height() - resizemargin) {
            return Area::BottomRight;
        } else if (y <= resizemargin) {
            return Area::TopRight;
        } else {
            return Area::Right;
        }
    } else {
        if (y >= rect.height() - resizemargin) {
            return Area::Bottom;
        } else if (y <= resizemargin) {
            return Area::Top;
        }
    }
    return Area::Center;
}

/**
 * 根据区域返回对应的光标形状
 */
Qt::CursorShape MainWindow::GetCursorForArea(Area area) {
    switch (area) {
        case Top: case Bottom:          return Qt::SizeVerCursor;
        case Left: case Right:          return Qt::SizeHorCursor;
        case TopLeft: case BottomRight: return Qt::SizeFDiagCursor;
        case TopRight: case BottomLeft: return Qt::SizeBDiagCursor;
        case Center:                    return Qt::SizeAllCursor;
        default:                        return Qt::ArrowCursor;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isMouseDown = true;
        m_pressRect = geometry();                       // 窗口的当前位置和尺寸(左上角坐标和宽高)
        m_pressPos = event->globalPosition().toPoint(); // 鼠标按下时的全局位置
        m_posx = mapFromGlobal(m_pressPos).x();         // 将全局坐标转换为相当于当前窗口左上角的局部坐标
        m_posy = mapFromGlobal(m_pressPos).y();
        m_area = GetArea(event->pos().x(), event->pos().y());
        setCursor(GetCursorForArea(m_area));
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!m_isMouseDown) return;

    auto currentPos = event->globalPosition().toPoint();
    auto moveSize = currentPos - m_pressPos;
    auto currentRect = m_pressRect;

    setCursor(GetCursorForArea(m_area));

    switch (m_area) {
    case Top:
        currentRect.setTop(m_pressRect.top() + moveSize.y());
        break;
    case Bottom:
        currentRect.setBottom(m_pressRect.bottom() + moveSize.y());
        break;
    case Left:
        currentRect.setLeft(m_pressRect.left() + moveSize.x());
        break;
    case Right:
        currentRect.setRight(m_pressRect.right() + moveSize.x());
        break;
    case TopLeft:
        currentRect.setTopLeft(m_pressRect.topLeft() + moveSize);
        break;
    case TopRight:
        currentRect.setTopRight(m_pressRect.topRight() + moveSize);
        break;
    case BottomLeft:
        currentRect.setBottomLeft(m_pressRect.bottomLeft() + moveSize);
        break;
    case BottomRight:
        currentRect.setBottomRight(m_pressRect.bottomRight() + moveSize);
        break;
    case Center:
        move(currentPos.x() - m_posx, currentPos.y() - m_posy);
        return;
    }

    const int minW = 700;
    const int minH = 500;
    bool needAdjust = false;
    if (currentRect.width() < minW) {
        needAdjust = true;
        if (currentRect.left() != m_pressRect.left()) {
            currentRect.setLeft(currentRect.right() - minW);
        } else {
            currentRect.setRight(currentRect.left() + minW);
        }
    }
    if (currentRect.height() < minH) {
        needAdjust = true;
        if (currentRect.top() != m_pressRect.top()) {
            currentRect.setTop(currentRect.bottom() - minH);
        } else {
            currentRect.setBottom(currentRect.top() + minH);
        }
    }
    if (needAdjust || currentRect != geometry()) {
        setGeometry(currentRect);
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    m_isMouseDown = false;
    unsetCursor();
}

void MainWindow::ShowAddFriendWindow() {
    m_addFriendWindow->show();
}

void MainWindow::SlotGetFriendInfoSuccess(const QByteArray& data) {
    emit SIG_GetFriendInfoSuccess(data);
}

void MainWindow::SlotReciveAddFriendReq(const QByteArray& data) {
    emit SIG_ReciveAddFriendReq(data);
}

void MainWindow::SlotSelectEvent() {
    // 取消右侧导航栏之前选中的按钮的选中状态，然后将当前的按钮设置为选中状态
    m_btn->setChecked(false);
    m_btn = (QPushButton*)sender();
    m_btn->setChecked(true);

    switch (m_page) {
    case ChatWidget:
        ui->gridLayout_2->removeWidget(m_chatPaneWidget);
        ui->gridLayout_3->removeWidget(m_chatMainWidget);
        m_chatPaneWidget->hide();
        m_chatMainWidget->hide();
        break;
    case Contactwidget:
        ui->gridLayout_2->removeWidget(m_contactsPaneWidget);
        // ui->gridLayout_3->removeWidget()
        m_contactsPaneWidget->hide();
        break;
    }

    auto name = m_btn->objectName();
    if (name == "btnAllChat") {
        m_page = ChatPage::ChatWidget;
        ui->gridLayout_2->addWidget(m_chatPaneWidget);
        ui->gridLayout_3->addWidget(m_chatMainWidget);
        m_chatPaneWidget->show();
        m_chatMainWidget->show();
    } else {
        m_page = ChatPage::Contactwidget;
        ui->gridLayout_2->addWidget(m_contactsPaneWidget);
        m_contactsPaneWidget->show();
    }

}