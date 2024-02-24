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


#define SIZE_ETH sizeof(struct ether_header)
#define SIZE_IPV4 sizeof(struct ip)
#define SIZE_IPV6 sizeof(struct ip6_hdr)
#define SIZE_TCP sizeof(struct tcphdr)
#define SIZE_UDP sizeof(struct udphdr)

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

    void readPcapFile(const string &pcapFile,
                      vector<CustomPacket *> &pv,
                      const unordered_map<string, tuple<int, string>> &logMap);
    static string formatTime(const struct pcap_pkthdr& header);
    static string removeYear(const pcap_pkthdr &header);

    static PcapReader& getInstance() {
        static PcapReader pcapReader;
        return pcapReader;
    }

private:
    string filename;
    pcap_t *descr;
    vector<CustomPacket*> _pv;
};

#endif //LAZYSHARK_PCAPREADER_H
