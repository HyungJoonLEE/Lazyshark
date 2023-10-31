#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    bool loadFile(const QString &fileName);


private slots:
    void on_submitBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
