#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

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
#include <QVector>

namespace Ui {
class AnalyzeWindow;
}

class AnalyzeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyzeWindow(QWidget *parent = nullptr);
    ~AnalyzeWindow();
    void setVector(const QVector<QString>& vec);
private:
    Ui::AnalyzeWindow *ui;
    QVector<QString> pcapFiles;
};

#endif // ANALYZEWINDOW_H
