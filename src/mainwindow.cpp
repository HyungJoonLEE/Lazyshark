#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Analyzewindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QImage image("./../images/main.png");
    QPainter p(&image);
    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Arial", 20, QFont::Bold));
    p.drawText(QRect(0, -10, width(), height()), Qt::AlignCenter,
               "Drag File Here");
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
    foreach(const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix().toLower() == "pcap") {
            qDebug() << "Dropped file:" << fileName;
            pcapVector.push_back(fileName);
        } else {
            QMessageBox::about(this, "WARNING", "Check extension is \'.pcap\'");
        }
    }
}


void MainWindow::processPcapFile(const int i, const std::string &pcapFile,
                                 PcapReader &PR,
                                 SnortRunner &SR,
                                 std::vector<CustomPacket *> &pv) {
    SR.generateSnortLog(pcapFile);
    if (PR.open(pcapFile))
        PR.readPcap(pcapFile, pv);
    else
        QMessageBox::warning(this, "Error",
                             "Something goes wrong while reading the file");
}


void MainWindow::on_submitBtn_clicked() {
    unsigned int fileCount = pcapVector.size();
    if (!fileCount) {
        QMessageBox::about(this, "WARNING", "No file detected");
    } else {
        AnalyzeWindow analyzeWindow[fileCount];
        auto &pcapReader = reinterpret_cast<PcapReader &>(PcapReader::getInstance());
        auto &snortRunner = reinterpret_cast<SnortRunner &>(SnortRunner::getInstance());

        for (int i = 0; i < fileCount; i++) {
            analyzeWindow[i].setVectorSize(DEFAULT_SIZE);
            const std::string pcapFile = pcapVector[i].toUtf8().constData();
            processPcapFile(i, pcapFile, pcapReader, snortRunner, analyzeWindow[i].getVector());
            analyzeWindow[i].printVector();
            analyzeWindow[i].setModal(true);
            analyzeWindow[i].exec();
        }
    }
}

