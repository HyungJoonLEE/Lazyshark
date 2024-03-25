#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QListWidget>
#include <QEvent>
#include <QMimeData>
#include <QPixmap>
#include <QImageReader>
#include <QDir>
#include <QButtonGroup>
#include <QPen>
#include <QPainter>
#include <QThread>
#include <cstdlib>
#include "PcapReader.h"
#include "SnortRunner.h"
#include "common.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void processPcapFile(const string &pcapFile,
                         PcapReader &PR, SnortRunner &SR,
                         vector<CustomPacket *> &pv);

private slots:
    void on_SubmitBtn_clicked();

    void on_RunBtn_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> pcapVector;
};
#endif // MAINWINDOW_H
