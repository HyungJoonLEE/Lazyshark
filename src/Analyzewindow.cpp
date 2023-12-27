#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"


AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeWindow)
{
    ui->setupUi(this);
}


AnalyzeWindow::~AnalyzeWindow() {
    for (auto it = m_av.begin(); it != m_av.end(); ++it) {
        CustomPacket* packet = *it;
        packet->clear();
    }
    delete ui;
}


void AnalyzeWindow::printVector() {
//    for (auto it = m_av.begin(); it != m_av.end(); ++it) {
//        CustomPacket* cp = *it;
//        std::cout << "Packet #" << cp->getNo() << std::endl;
//        std::cout << "Source IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_src) << std::endl;
//        std::cout << "Destination IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_dst) << std::endl;
//        std::cout << "Protocol: " << (unsigned short) cp->getIpv4Hdr()->ip_p << std::endl;
//        std::cout << "Length: " << cp->getIpv4Hdr()->ip_hl * 4 << std::endl;
//        std::cout << "Total Length: " << (short) ntohs(cp->getIpv4Hdr()->ip_len) << std::endl;
//        std::cout << "Source Port: " << ntohs(cp->getTCPHdr()->th_sport) << std::endl;
//        std::cout << "Destination Port: " << ntohs(cp->getTCPHdr()->th_dport) << "\n" << std::endl;
//    }
    size_t rowCount = m_av.size();
    ui->tableWidget->setRowCount(rowCount);
    for (int i = 0; i < rowCount; i++) {
        const char* ipSrc = inet_ntoa(m_av[i]->getIpv4Hdr()->ip_src);
        const char* ipDst = inet_ntoa(m_av[i]->getIpv4Hdr()->ip_dst);
        unsigned int tcpDataLen = 0, udpDataLen = 0;


        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString(m_av[i]->getTime()->c_str())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_av[i]->getIpv4Hdr()->ip_p)));
        if (m_av[i]->getIpv4Hdr()->ip_v == IPPROTO_IPV4) {
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromLatin1(ipSrc)));
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromLatin1(ipDst)));
            if (m_av[i]->getIpv4Hdr()->ip_p == IPPROTO_TCP) {
                tcpDataLen = ntohs(m_av[i]->getIpv4Hdr()->ip_len) - m_av[i]->getIpv4Hdr()->ip_hl * 4 - m_av[i]->getTCPHdr()->th_off * 4;
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ntohs(m_av[i]->getTCPHdr()->th_sport))));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(ntohs(m_av[i]->getTCPHdr()->th_dport))));
                ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(tcpDataLen)));
            }
            if (m_av[i]->getIpv4Hdr()->ip_p == IPPROTO_UDP) {
                udpDataLen = ntohs(m_av[i]->getIpv4Hdr()->ip_len) - m_av[i]->getIpv4Hdr()->ip_hl * 4 - sizeof(struct udphdr);
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ntohs(m_av[i]->getUDPHdr()->uh_sport))));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(ntohs(m_av[i]->getUDPHdr()->uh_dport))));
                ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(udpDataLen)));
            }
        }
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString("TEST HELLO")));
    }
}


void AnalyzeWindow::setVectorSize(size_t num) {
    m_av.reserve(num);
}


std::vector<CustomPacket *>& AnalyzeWindow::getVector() {
    return m_av;
}
