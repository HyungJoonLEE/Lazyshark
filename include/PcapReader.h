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
#include "Analyzewindow.h"
#include "PacketVector.h"



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

    bool open(const std::string &pcapFile);
    void close();

    void readPcap(const std::string &pcapFile);
    bool readNextPacket();

    static PcapReader& getInstance() {
        static PcapReader pcapReader;
        return pcapReader;
    }

private:
    static void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);

    std::string filename;
    pcap_t *descr;
};

#endif //LAZYSHARK_PCAPREADER_H
