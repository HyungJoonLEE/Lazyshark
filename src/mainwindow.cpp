#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_SubmitBtn_clicked()
{
    hide();
    QMessageBox::about(this, "SUBMIT", "SUBMIT SHIT");
}


void MainWindow::on_RunBtn_clicked()
{
    hide();
    QMessageBox::about(this, "RUN", "RUN ANUSHK");
}

