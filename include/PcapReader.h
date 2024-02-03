#ifndef LAZYSHARK_PCAPREADER_H
#define LAZYSHARK_PCAPREADER_H


#include <pcap.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>  // For ETH_P_IP
#include <netinet/ip.h>        // For struct ip
#include <netinet/tcp.h>       // For struct tcphdr
#include <netinet/udp.h>
#include <vector>
#include "CustomPacket.h"



extern "C"
{
pcap_t	*pcap_open_offline(const char *, char *);
};

class PcapReader {
public:
    PcapReader();
    PcapReader(const PcapReader& ref);
    PcapReader& operator=(const PcapReader& ref) {}
    ~PcapReader();

    bool open(const string &pcapFile);
    void close();

    void readPcap(const string &pcapFile);
    bool readNextPacket();

    static PcapReader& getInstance() {
        static PcapReader pcapReader;
        return pcapReader;
    }

private:
    static void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);

    string filename;
    pcap_t *descr;
    vector<CustomPacket*> packetVector;
};

#endif //LAZYSHARK_PCAPREADER_H
