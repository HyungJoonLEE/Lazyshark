#ifndef LAZYSHARK_COMMON_H
#define LAZYSHARK_COMMON_H

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

#include <iostream>
#include <utility>

#define SIZE_ETH sizeof(struct ether_header)
#define SIZE_IPV4 sizeof(struct ip)
#define SIZE_IPV6 sizeof(struct ip6_hdr)
#define SIZE_TCP sizeof(struct tcphdr)
#define SIZE_UDP sizeof(struct udphdr)

using namespace std;

extern unordered_map<int, string> portMap;

void populatePortMap();




#endif //LAZYSHARK_COMMON_H
