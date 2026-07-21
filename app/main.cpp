#include "loginwidget.h"
#include "mainwindow.h"
#include "logger.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDir>
#include "appcore.h"
#include "dbmagr.h"

int main(int argc, char *argv[])
{
    // Logger::initLog("run.log");
    QApplication a(argc, argv);
    // a.setStyle(QStyleFactory::create("macOS"));

    /* 加载样式 */
    QFile file(":/qss/resource/qss/default.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    QString dbPath = "./data/";
    QDir dir(dbPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "无法创建目录:" << dir.absolutePath();
            return -1;
        }
    }
    qDebug() << "当前工作目录:" << QDir::currentPath();
    qDebug() << "数据库路径:" << QFileInfo(dbPath).absoluteFilePath();
    DBMagr::Instance().Init();
    DBMagr::Instance().OpenConnection("userConn", "./data/welink.db");

    // LoginWidget w;
    // w.show();
    AppCore appCore;
    // MainWindow w;
    // w.show();

    auto ret = QCoreApplication::exec();

    // Logger::unInitLog(); // 退出关闭日志

    return ret;
}
