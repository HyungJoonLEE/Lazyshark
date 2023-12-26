#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"
#include "PacketVector.h"



AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(10000);
    for (int i = 0; i < 4000; i++) {
        for (int j = 0; j < 9; j++) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString()));
        }
    }


}


AnalyzeWindow::~AnalyzeWindow() {
    delete ui;
}
