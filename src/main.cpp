#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.connect(&a, &QApplication::aboutToQuit, &w, &MainWindow::aboutToQuit);
    w.show();

    return a.exec();
}
