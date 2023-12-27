#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DEFAULT_SIZE 10000

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
#include <cstdlib>
#include "PcapReader.h"
#include "SnortRunner.h"


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
    void processPcapFile(const int i, const std::string &pcapFile,
                         PcapReader& PR, SnortRunner& SR, std::vector<CustomPacket *> &pv);

private slots:
    void on_submitBtn_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> pcapVector;
};
#endif // MAINWINDOW_H
