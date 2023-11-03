#ifndef LAZYSHARK_FILEIO_H
#define LAZYSHARK_FILEIO_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class FileIO
{

public:
    void readFile(const QVector<QString>& pcapList);

private:

};

#endif //LAZYSHARK_FILEIO_H
