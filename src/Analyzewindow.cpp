#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"
#include "PcapReader.h"


AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::AnalyzeWindow) {

    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setVisible(false);
    initPriorityMap();

    // set table
    ui->tableWidget->setColumnWidth(0, 60);     // NO
    ui->tableWidget->setColumnWidth(1, 200);    // TIME
    ui->tableWidget->setColumnWidth(2, 60);     // PROTOCOL
    ui->tableWidget->setColumnWidth(3, 150);    // SOURCE
    ui->tableWidget->setColumnWidth(4, 70);     // S_PORT
    ui->tableWidget->setColumnWidth(5, 70);     // D_PORT
    ui->tableWidget->setColumnWidth(6, 150);    // DESTINATION
    ui->tableWidget->setColumnWidth(7, 60);     // LENGTH
    ui->tableWidget->setColumnWidth(8, 200);    // WARNING
    ui->tableWidget->setColumnWidth(9, 700);    // DATA
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
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::fromStdString(_rv[i]->getData())));
        if (!_rv[i]->getWarning().empty()) {
            QColor color(priorityMap_[_rv[i]->getPriority()].c_str());
            for (int j = 0; j < 10; j++) {
                ui->tableWidget->item(i, j)->setBackground(color);
            }
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

