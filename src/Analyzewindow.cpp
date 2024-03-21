#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"
#include "PcapReader.h"


AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::AnalyzeWindow) {

    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setVisible(false);
    initPriorityMap();

    // set table
    ui->tableWidget->setColumnWidth(0, 90);     // NO
    ui->tableWidget->setColumnWidth(1, 250);    // TIME
    ui->tableWidget->setColumnWidth(2, 100);     // PROTOCOL
    ui->tableWidget->setColumnWidth(3, 200);    // SOURCE
    ui->tableWidget->setColumnWidth(4, 90);     // S_PORT
    ui->tableWidget->setColumnWidth(5, 90);     // D_PORT
    ui->tableWidget->setColumnWidth(6, 200);    // DESTINATION
    ui->tableWidget->setColumnWidth(7, 80);     // LENGTH
    ui->tableWidget->setColumnWidth(8, 300);    // WARNING
//    ui->tableWidget->setColumnWidth(9, 700);    // DATA
}


AnalyzeWindow::~AnalyzeWindow() {
    delete ui;
}


void AnalyzeWindow::fillTable() {
    size_t rowCount = _rv.size();
    ui->tableWidget->setRowCount((int) rowCount);
    for (int i = 0; i < rowCount; i++) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(_rv[i]->getNo())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(_rv[i]->getTime())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(_rv[i]->getProtocol())));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(_rv[i]->getSIP())));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(_rv[i]->getDIP())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_rv[i]->getSPort())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(_rv[i]->getDPort())));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(_rv[i]->getLen())));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(_rv[i]->getWarning())));
//        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::fromStdString(_rv[i]->getData())));
        ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 8)->setTextAlignment(Qt::AlignCenter);
//        ui->tableWidget->item(i, 9)->setTextAlignment(Qt::AlignCenter);
        if (!_rv[i]->getWarning().empty()) {
            QColor color(priorityMap_[_rv[i]->getPriority()].c_str());
            for (int j = 0; j < 9; j++) {
                ui->tableWidget->item(i, j)->setBackground(color);}
        }
    }
}

vector<CustomPacket *>& AnalyzeWindow::getRv() {
    return _rv;
}


void AnalyzeWindow::initPriorityMap() {
    priorityMap_[1] = "#FF0000";    // red
    priorityMap_[2] = "#FFD700";    // yellow
    priorityMap_[3] = "#ADFF2F";    // green-yellow
    priorityMap_[4] = "#98FB98";    // light-green
}

