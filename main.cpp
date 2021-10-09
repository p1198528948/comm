#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include "CWidCenterCtrl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QStringList strStyleList = QStyleFactory::keys();
    //QApplication::setStyle(QStyleFactory::create(strStyleList.at(2)));

    CWidCenterCtrl w;
    w.resize(800, 600);
    w.show();

    return a.exec();
}
