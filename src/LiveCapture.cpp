#include "LiveCapture.h"
#include "ui_LiveCapture.h"


LiveCapture::LiveCapture(QWidget *parent) :
    QDialog(parent), ui(new Ui::LiveCapture) {

    initPriorityMap();

    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->setColumnWidth(0, 90);     // NO
    ui->tableWidget->setColumnWidth(1, 250);    // TIME
    ui->tableWidget->setColumnWidth(2, 100);     // PROTOCOL
    ui->tableWidget->setColumnWidth(3, 200);    // SOURCE
    ui->tableWidget->setColumnWidth(4, 90);     // S_PORT
    ui->tableWidget->setColumnWidth(5, 90);     // D_PORT
    ui->tableWidget->setColumnWidth(6, 200);    // DESTINATION
    ui->tableWidget->setColumnWidth(7, 80);     // LENGTH
    ui->tableWidget->setColumnWidth(8, 400);    // WARNING

    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &LiveCapture::onItemClicked);
}


LiveCapture::~LiveCapture() {
    delete ui;
}


void LiveCapture::onItemClicked(QTableWidgetItem *item) {

}


void LiveCapture::initPriorityMap() {
    priorityMap_[1] = "#FF0000";    // red
    priorityMap_[2] = "#FFD700";    // yellow
    priorityMap_[3] = "#ADFF2F";    // green-yellow
    priorityMap_[4] = "#2FE3FF";    // light-green
}
