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
#include <netinet/ip_icmp.h>
#include <vector>
#include <sstream>
#include "CustomPacket.h"
#include "common.h"


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
    static string removeYear(const string &time);


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
