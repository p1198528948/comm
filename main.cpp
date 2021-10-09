#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList strStyleList = QStyleFactory::keys();
    QApplication::setStyle(QStyleFactory::create(strStyleList.at(2)));
    MainWindow w;
    w.show();

    return a.exec();
}
