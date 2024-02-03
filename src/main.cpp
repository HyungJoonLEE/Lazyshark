#include "mainwindow.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication project(argc, argv);
    MainWindow w;
    w.show();
    return project.exec();
}
