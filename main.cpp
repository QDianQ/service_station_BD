#include "mainwindow.h"
#include "testmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    w.showMaximized();

//    TestMainWindow testWindow;
//    testWindow.show();
    return a.exec();
}
