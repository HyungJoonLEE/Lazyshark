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
#include "PacketCapturer.h"


namespace Ui {
    class LiveCapture;
}

class LiveCapture : public QDialog
{
    Q_OBJECT

public:
    explicit LiveCapture(PacketCapturer* pc, QWidget *parent = nullptr);
    ~LiveCapture();

    vector<CustomPacket*>& get_cv() { return cv; }
    void refreshTable();

private slots:
    void onItemClicked(QTableWidgetItem* item);

private:
    PacketCapturer pc;
    Ui::LiveCapture *ui;
    unordered_map<int, string> priorityMap_;
    vector<CustomPacket*> cv;
    unsigned long priv = 0;

};
#endif //LIVECAPTURE_H
