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


extern "C"
{
pcap_t	*pcap_open_offline(const char *, char *);
};

class PcapReader {
public:
    PcapReader() {}
    PcapReader(const std::string &filename);
    PcapReader(const PcapReader& ref) {}
    PcapReader& operator= (const PcapReader& ref) {}
    ~PcapReader();

    bool open();
    void close();
    void pcapRead();
    bool readNextPacket();
    void setFileName(std::string &pcapFile);

    static PcapReader& getInstance(const std::string& fileName) {
        static PcapReader pcapReader(fileName);
        return pcapReader;
    }

private:
    static void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);

    std::string filename;
    pcap_t *descr;
};

#endif //LAZYSHARK_PCAPREADER_H
