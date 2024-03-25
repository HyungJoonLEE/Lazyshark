#ifndef LAZYSHARK_PACKETCAPTURER_H
#define LAZYSHARK_PACKETCAPTURER_H

#include <QObject>
#include <pcap.h>

class PacketCapturer : public QObject {
Q_OBJECT
public:
    explicit PacketCapturer(QObject *parent = nullptr);
    void startCapture(); // Function to start capturing packets

signals:
    void newPacket(QString info); // Signal to emit when a new packet is captured
};

void packetHandler(u_char *userData, const struct pcap_pkthdr *packetHeader, const u_char *packetData);

#endif //LAZYSHARK_PACKETCAPTURER_H
