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
    ui->tableWidget->setColumnWidth(8, 400);    // WARNING
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

        // color warnings
        if (!_rv[i]->getWarning().empty()) {
            QColor color(priorityMap_[_rv[i]->getPriority()].c_str());
            for (int j = 0; j < 9; j++) {
                ui->tableWidget->item(i, j)->setBackground(color);
            }
        }
        protocols.insert(_rv[i]->getProtocol());
        ips.insert(_rv[i]->getSIP());
        ips.insert(_rv[i]->getDIP());
        ports.insert(to_string(_rv[i]->getSPort()));
        ports.insert(to_string(_rv[i]->getDPort()));
    }
    
    // Drop down filter
    ui->comboBox->addItem(QString::fromStdString("ALL"));
    for (const auto &item : protocols) {
        ui->comboBox->addItem(QString::fromStdString(item));
    }

    // IP filter
    ui->comboBox_2->addItem(QString::fromStdString("ALL"));
    for (const auto &item : ips) {
        ui->comboBox_2->addItem(QString::fromStdString(item));
    }

    ui->comboBox_3->addItem(QString::fromStdString("ALL"));
    for (const auto &item : ports) {
        ui->comboBox_3->addItem(QString::fromStdString(item));
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


void AnalyzeWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    protocol_val = arg1;
}


void AnalyzeWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    ip_val = arg1;
}


void AnalyzeWindow::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    port_val = arg1;
}

void AnalyzeWindow::on_pushButton_clicked() {
    ui->tableWidget->clearContents();
    size_t rowCount = _rv.size();
    ui->tableWidget->setRowCount((int) rowCount);

    int g = 0;
    for (size_t h = 0; h < rowCount; ++h) {
        bool protocolMatch = (protocol_val == "ALL") || (QString::fromStdString(_rv[h]->getProtocol()) == protocol_val);
        bool ipMatch = (ip_val == "ALL") || (QString::fromStdString(_rv[h]->getSIP()) == ip_val || QString::fromStdString(_rv[h]->getDIP()) == ip_val);
        bool portMatch = (port_val == "ALL") || (QString::number(_rv[h]->getSPort()) == port_val || QString::number(_rv[h]->getDPort()) == port_val);
        bool hasWarning = !_rv[h]->getWarning().empty(); // Check if there is a warning
        bool justWarning = (ui->comboBox_4->currentText() == "ALL") || hasWarning;

        if (protocolMatch && ipMatch && portMatch && justWarning) {
            ui->tableWidget->setItem(g, 0, new QTableWidgetItem(QString::number(_rv[h]->getNo())));
            ui->tableWidget->setItem(g, 1, new QTableWidgetItem(QString::fromStdString(_rv[h]->getTime())));
            ui->tableWidget->setItem(g, 2, new QTableWidgetItem(QString::fromStdString(_rv[h]->getProtocol())));
            ui->tableWidget->setItem(g, 3, new QTableWidgetItem(QString::fromStdString(_rv[h]->getSIP())));
            ui->tableWidget->setItem(g, 4, new QTableWidgetItem(QString::number(_rv[h]->getSPort())));
            ui->tableWidget->setItem(g, 5, new QTableWidgetItem(QString::number(_rv[h]->getDPort())));
            ui->tableWidget->setItem(g, 6, new QTableWidgetItem(QString::fromStdString(_rv[h]->getDIP())));
            ui->tableWidget->setItem(g, 7, new QTableWidgetItem(QString::number(_rv[h]->getLen())));
            ui->tableWidget->setItem(g, 8, new QTableWidgetItem(QString::fromStdString(_rv[h]->getWarning())));

            ui->tableWidget->item(g, 0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 2)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 3)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 4)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 5)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 6)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 7)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(g, 8)->setTextAlignment(Qt::AlignCenter);

            if (!_rv[h]->getWarning().empty()) {
                QColor color(priorityMap_[_rv[h]->getPriority()].c_str());
                for (int k = 0; k < 9; ++k) {
                    ui->tableWidget->item(g, k)->setBackground(color);
                }
            }
            g++;

        }
    }
}


