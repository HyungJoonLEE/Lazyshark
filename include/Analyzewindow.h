#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include <QMessageBox>
#include <QListWidget>
#include <QMimeData>
#include <QDir>
#include <QVector>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include "PcapReader.h"

namespace Ui {
class AnalyzeWindow;
}

class AnalyzeWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AnalyzeWindow(QWidget *parent = nullptr);
    ~AnalyzeWindow();
    void setupTable();

private:
    Ui::AnalyzeWindow *ui;
    QTableWidget* table;
};

#endif // ANALYZEWINDOW_H
