#include "mainwindow.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication project(argc, argv);
    MainWindow w;
    w.show();
    return project.exec();
}
