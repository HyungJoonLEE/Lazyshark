#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Analyzewindow.h"
#include "PcapReader.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage image("./../images/main.png");
    QPainter p(&image);
    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Arial", 20, QFont::Bold));
    p.drawText(QRect(0, -10, width(), height()), Qt::AlignCenter, "Drag File Here");
    ui->mainPic->setPixmap(QPixmap::fromImage(image));

    QIcon ButtonIcon(QPixmap("./../images/runIcon.png"));
    ui->scanBtn->setIcon(ButtonIcon);
    ui->scanBtn->setIconSize(QSize(30, 30));
    setAcceptDrops(true);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}


void MainWindow::dropEvent(QDropEvent *e) {
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix().toLower() == "pcap") {
            qDebug() << "Dropped file:" << fileName;
            pcapVector.push_back(fileName);
        }
        else {
            QMessageBox::about(this, "WARNING", "Check extension is \'.pcap\'");
        }
    }
}


void MainWindow::on_submitBtn_clicked() {
    unsigned int fileCount = pcapVector.size();
    if (!fileCount) {
        QMessageBox::about(this, "WARNING", "No file detected");
    }
    else {
        auto& pcapReader = reinterpret_cast<PcapReader &>(PcapReader::getInstance());
        AnalyzeWindow analyzeWindow[fileCount];
        for (int i = 0; i < fileCount; i++) {
            std::string pcapFile = pcapVector.at(i).toUtf8().constData();
            generateSnortLog(pcapFile);
            if (pcapReader.open(pcapFile)) {
                pcapReader.pcapRead(pcapFile);
            }
            else {
                QMessageBox::warning(this, "Error", "Something goes wrong while reading the file");
            }
            analyzeWindow[i].setModal(true);
            analyzeWindow[i].exec();
        }
    }
}


void MainWindow::generateSnortLog(std::string &pcapFile) {
    std::string cmd = "snort -c ./../snort/etc/snort.conf -r '"
                      + pcapFile
                      + "' -l ./../logs";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Failed to open pipe\n";
        return;
    }
}



