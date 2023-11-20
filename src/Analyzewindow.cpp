#include "Analyzewindow.h"
#include "ui_Analyzewindow.h"
#include "PcapReader.h"


AnalyzeWindow::AnalyzeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeWindow)
{
    ui->setupUi(this);
}


AnalyzeWindow::~AnalyzeWindow() {
    delete ui;
}


void AnalyzeWindow::setVector(const QVector<QString>& pcapVector) {
    for (const QString& fileName : pcapVector) {
        pcapFiles.push_back(fileName);
    }
}


void AnalyzeWindow::readPcapFile(std::string& pcapFile) {
    PcapReader reader(pcapFile);
    if (reader.open()) {
        reader.pcapRead();
    }
}



