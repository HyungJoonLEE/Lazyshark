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

