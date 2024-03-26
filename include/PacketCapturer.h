#ifndef LAZYSHARK_PACKETCAPTURER_H
#define LAZYSHARK_PACKETCAPTURER_H

#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>  // For ETH_P_IP
#include <netinet/ip.h>        // For struct ip
#include <netinet/tcp.h>       // For struct tcphdr
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <vector>
#include <sstream>
#include <pcap/pcap.h>
#include "CustomPacket.h"
#include "common.h"

using namespace std;

class PacketCapturer : public QObject {
Q_OBJECT

public:
    explicit PacketCapturer(QObject *parent = nullptr);
    void startCapture(vector<CustomPacket *> &cv); // Function to start capturing packets
    void notifyPacketCaptured();

signals:
    void cvUpdated(); // Signal to emit when a new packet is captured
};

string formatTime(const pcap_pkthdr* &header);
void packetHandler(u_char *userData, const struct pcap_pkthdr *packetHeader, const u_char *packetData);
string removeYear(const string &time);


#endif //LAZYSHARK_PACKETCAPTURER_H
