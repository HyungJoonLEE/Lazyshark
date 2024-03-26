#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Analyzewindow.h"
#include "LiveCapture.h"
#include "PacketCapturer.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

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
            QMessageBox::about(this,
                               "WARNING",
                               "Check extension is \'.pcap\'");
        }
    }
}


void MainWindow::processPcapFile(const string &pcapFile,
                                 PcapReader &PR,
                                 SnortRunner &SR,
                                 vector<CustomPacket *>& av) {
    SR.generateSnortLog(pcapFile);
    SR.processLog(pcapFile);
    if (PR.open(pcapFile))
        PR.readPcapFile(pcapFile, av, SR.getLogMap());
    else
        QMessageBox::warning(this,
                             "Error",
                             "Something goes wrong while reading the file");
}


void MainWindow::on_SubmitBtn_clicked() {
    unsigned int fileCount = pcapVector.size();

    if (!fileCount)
        QMessageBox::about(this,
                           "WARNING",
                           "No file detected");
    else {
        vector<std::unique_ptr<AnalyzeWindow>> AW;
        populatePortMap();
        auto &PR = reinterpret_cast<PcapReader &>(PcapReader::getInstance());
        auto &SR = reinterpret_cast<SnortRunner &>(SnortRunner::getInstance());

        for (int i = 0; i < fileCount; i++) {
            AW.emplace_back(make_unique<AnalyzeWindow>());
            const string pcapFile = pcapVector[i].toUtf8().constData();
            processPcapFile(pcapFile, PR, SR, AW[i]->getRv());
            AW[i]->setModal(true);
            AW[i]->showMaximized();
            AW[i]->fillTable();
            AW[i]->exec();
        }
    }
}


void MainWindow::on_RunBtn_clicked() {
    auto *pc = new PacketCapturer();
    auto *lc = new LiveCapture(pc);
    lc->setModal(true);
    lc->showMaximized();

    auto *pc_thread = new QThread();
//    auto *lc_thread = new QThread();

    pc->moveToThread(pc_thread);
//    pc->moveToThread(lc_thread);

//    connect(lc_thread, &QThread::started, lc, &LiveCapture::exec);
    connect(pc_thread, &QThread::started, pc, [pc, lc]() {
        pc->startCapture(lc->get_cv());
    });

    pc_thread->start();
//    lc_thread->start();
    lc->exec();
}

