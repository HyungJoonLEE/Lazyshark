#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include <QMessageBox>
#include <QListWidget>
#include <QMimeData>
#include <QDir>
#include <QVector>
#include <QString>
#include <QHeaderView>
#include <unordered_map>
#include <string>
#include "CustomPacket.h"


namespace Ui {
class AnalyzeWindow;
}

class AnalyzeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyzeWindow(QWidget *parent = nullptr);
    ~AnalyzeWindow();
    void fillTable();
    void initPriorityMap();
    vector<CustomPacket*>& getRv();
private:
    Ui::AnalyzeWindow *ui;
    QVector<QString> pcapFiles;
    vector<CustomPacket *> _rv;
    unordered_map<int, string> priorityMap_;
};

#endif // ANALYZEWINDOW_H
