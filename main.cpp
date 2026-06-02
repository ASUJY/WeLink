#include "loginwidget.h"
#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // a.setStyle(QStyleFactory::create("macOS"));

    LoginWidget w;
    w.show();

    return QCoreApplication::exec();
}
