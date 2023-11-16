#ifndef LAZYSHARK_PCAPREADER_H
#define LAZYSHARK_PCAPREADER_H

#include <pcap.h>
#include <string>

class PcapReader {
public:
    PcapReader(const std::string &filename);
    ~PcapReader();

    bool open();
    void close();
    bool readNextPacket();

private:
    static void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);

    std::string filename;
    pcap_t *descr;
};

#endif //LAZYSHARK_PCAPREADER_H
