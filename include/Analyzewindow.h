#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include <QMessageBox>
#include <QListWidget>
#include <QMimeData>
#include <QDir>
#include <QVector>
#include <QString>

namespace Ui {
class AnalyzeWindow;
}

class AnalyzeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyzeWindow(QWidget *parent = nullptr);
    ~AnalyzeWindow();
private:
    Ui::AnalyzeWindow *ui;
    QVector<QString> pcapFiles;
};

#endif // ANALYZEWINDOW_H
