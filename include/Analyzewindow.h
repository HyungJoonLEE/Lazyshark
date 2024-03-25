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
#include <set>
#include <QTableWidgetItem>
#include "CustomPacket.h"
#include "HexDumpDialog.h"


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
private slots:

    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_comboBox_2_currentTextChanged(const QString &arg1);
    void on_comboBox_3_currentTextChanged(const QString &arg1);
    void on_pushButton_clicked();
    void onItemClicked(QTableWidgetItem* item);

private:
    Ui::AnalyzeWindow *ui;
    QVector<QString> pcapFiles;

    vector<CustomPacket *> _rv;
    unordered_map<int, string> priorityMap_;

    // filters
    set<string> protocols;
    set<string> ips;
    set<string> ports;

    // filter value
    QString protocol_val = QString::fromStdString("ALL");
    QString ip_val = QString::fromStdString("ALL");
    QString port_val = QString::fromStdString("ALL");
};

#endif // ANALYZEWINDOW_H
