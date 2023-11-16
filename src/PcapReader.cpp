#include "PcapReader.h"
#include <iostream>

PcapReader::PcapReader(const std::string &filename) : filename(filename), descr(nullptr) {}

PcapReader::~PcapReader() {
    close();
}

bool PcapReader::open() {
    char errbuf[PCAP_ERRBUF_SIZE];
    descr = pcap_open_offline(filename.c_str(), errbuf);
    if (descr == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return false;
    }
    return true;
}

void PcapReader::close() {
    if (descr != nullptr) {
        pcap_close(descr);
        descr = nullptr;
    }
}

bool PcapReader::readNextPacket() {
    return pcap_loop(descr, 1, packetHandler, reinterpret_cast<u_char*>(this)) >= 0;
}

void PcapReader::packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Cast userData to PcapReader instance and process the packet
    PcapReader *reader = reinterpret_cast<PcapReader*>(userData);
    std::cout << "Packet length: " << pkthdr->len << std::endl;
    // Add more packet processing code here
}
