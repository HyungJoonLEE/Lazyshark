#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Analyzewindow.h"


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
    if (pcapVector.empty()) {
        QMessageBox::about(this, "WARNING", "No file detected");
    }
    else {
        AnalyzeWindow analyzeWindow;
        analyzeWindow.setModal(true);
        analyzeWindow.setVector(getPcapVector());
        analyzeWindow.exec();
    }
}


QVector<QString> &MainWindow::getPcapVector() {
    return pcapVector;
}

