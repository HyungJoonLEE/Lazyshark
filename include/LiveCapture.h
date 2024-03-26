#ifndef LIVECAPTURE_H
#define LIVECAPTURE_H

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
    class LiveCapture;
}

class LiveCapture : public QDialog
{
    Q_OBJECT

public:
    explicit LiveCapture(QWidget *parent = nullptr);
    ~LiveCapture();

    void initPriorityMap();
    vector<CustomPacket*>& get_cv() { return cv; }

private slots:
    void onItemClicked(QTableWidgetItem* item);


private:
    Ui::LiveCapture *ui;
    unordered_map<int, string> priorityMap_;
    vector<CustomPacket*> cv;

};
#endif //LIVECAPTURE_H
