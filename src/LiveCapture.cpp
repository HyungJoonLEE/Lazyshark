#include "LiveCapture.h"
#include "ui_LiveCapture.h"


LiveCapture::LiveCapture(PacketCapturer* pc, QWidget *parent) :
    QDialog(parent), ui(new Ui::LiveCapture), pc(pc) {

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
    if (pc) {
        connect(pc, &PacketCapturer::cvUpdated, this, &LiveCapture::refreshTable);
    }
}


LiveCapture::~LiveCapture() {
    delete ui;
}


void LiveCapture::refreshTable() {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(cv.size());

    for (int i = 0; i < cv.size(); ++i) {
        const CustomPacket* packet = cv.at(i);

        // Assuming CustomPacket class has methods to get necessary info
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(packet->getNo())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(packet->getTime())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(packet->getProtocol())));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(packet->getSIP())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(packet->getSPort())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(packet->getDPort())));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(packet->getDIP())));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(packet->getLen())));

        ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);
    }
}

void LiveCapture::onItemClicked(QTableWidgetItem *item) {
    int target = 0;
    if (item != nullptr) {
        QString value = item->text();
        int row = item->row();

        QTableWidgetItem *itemInSpecificColumn = ui->tableWidget->item(row, 0);
        if (itemInSpecificColumn != nullptr) {
            QString value = itemInSpecificColumn->text();
            bool col_exist;
            int dataIndex = value.toInt(&col_exist) - 1;
            if (col_exist) {
                auto *dialog = new HexDumpDialog(QString::fromStdString(cv[dataIndex]->getData()));
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->exec();
            }
            else {
                qDebug() << "Failed to convert QString to int.";
            }
        }
    }
}

