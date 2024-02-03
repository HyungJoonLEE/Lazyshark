#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include <QMessageBox>
#include <QListWidget>
#include <QMimeData>
#include <QDir>
#include <QVector>
#include <QString>
#include <QHeaderView>
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
private:
    Ui::AnalyzeWindow *ui;
    QVector<QString> pcapFiles;
    vector<CustomPacket*> m_av;
};

#endif // ANALYZEWINDOW_H
