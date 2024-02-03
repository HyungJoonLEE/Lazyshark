#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"
#include "PcapReader.h"


    AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::AnalyzeWindow)
{
    ui->setupUi(this);

    // set table
    ui->tableWidget->setColumnWidth(0, 100);     // NO
    ui->tableWidget->setColumnWidth(1, 200);     // TIME
    ui->tableWidget->setColumnWidth(2, 100);     // PROTOCOL
    ui->tableWidget->setColumnWidth(3, 150);     // SOURCE
    ui->tableWidget->setColumnWidth(4, 100);     // S_PORT
    ui->tableWidget->setColumnWidth(5, 100);     // D_PORT
    ui->tableWidget->setColumnWidth(6, 150);     // DESTINATION
    ui->tableWidget->setColumnWidth(7, 100);     // LENGTH
    ui->tableWidget->setColumnWidth(8, 200);     // INFO
    ui->tableWidget->setColumnWidth(9, 300);     // WARNING
}


AnalyzeWindow::~AnalyzeWindow() {
    delete ui;
}


void AnalyzeWindow::fillTable() {
    size_t rowCount = m_av.size();
    ui->tableWidget->setRowCount(rowCount);
    for (int i = 0; i < rowCount; i++) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_av[i]->getNo())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("TEST TIME")));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(m_av[i]->getIpv4Hdr()->ip_p)));
        // Convert 'in_addr' to a C string
        const char* ipSrc = inet_ntoa(m_av[i]->getIpv4Hdr()->ip_src);
        const char* ipDst = inet_ntoa(m_av[i]->getIpv4Hdr()->ip_dst);
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromLatin1(ipSrc)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(m_av[i]->getTCPHdr()->th_sport)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(m_av[i]->getTCPHdr()->th_dport)));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::fromLatin1(ipDst)));
        unsigned int plen = m_av[i]->getIpv4Hdr()->ip_len - m_av[i]->getIpv4Hdr()->ip_hl - m_av[i]->getTCPHdr()->doff * 4;
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(plen)));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString("TEST HELLO")));
    }
}

