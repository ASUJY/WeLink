#include "loginwidget.h"
#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include "appcore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // a.setStyle(QStyleFactory::create("macOS"));

    /* 加载样式 */
    QFile file(":/qss/resource/qss/default.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    // LoginWidget w;
    // w.show();
    AppCore appCore;

    return QCoreApplication::exec();
}
