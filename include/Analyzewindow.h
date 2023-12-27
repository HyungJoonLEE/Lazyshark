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
#include <netinet/in.h>
#include <arpa/inet.h>
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
    void setupTable();
    void printVector();
    void setVectorSize(size_t num);
    std::vector<CustomPacket*>& getVector();

private:
    Ui::AnalyzeWindow *ui;
    QTableWidget* table;
    std::vector<CustomPacket*> m_av;
};

#endif // ANALYZEWINDOW_H
