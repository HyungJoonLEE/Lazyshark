#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->mainPic->setPixmap(QPixmap("/Users/Klaiden/Desktop/Lazyshark/images/cloud3.png"));

    QImage image("./../images/cloud2.png");
    QPainter p(&image);
    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Arial", 20, QFont::Bold));
    p.drawText(QRect(0,-10,width(),height()), Qt::AlignCenter, "Drag File Here");
    ui->mainPic->setPixmap(QPixmap::fromImage(image));

    QIcon ButtonIcon(QPixmap("./../images/cute.png"));
    ui->scanBtn->setIcon(ButtonIcon);
    ui->scanBtn->setIconSize(QSize(30, 30));
    setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
    }
}

void MainWindow::on_submitBtn_clicked()
{
//    hide();
    QMessageBox::about(this, "SUBMIT", "SUBMIT SHIT");
}

