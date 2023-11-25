#include "PcapReader.h"


PcapReader::PcapReader() :  descr(nullptr) {}


PcapReader::~PcapReader() {
    close();
}


bool PcapReader::open(std::string &pcapFile) {
    char errbuf[PCAP_ERRBUF_SIZE];
    descr = pcap_open_offline(pcapFile.c_str(), errbuf);
    if (descr == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return false;
    }
    return true;
}


void PcapReader::pcapRead(const std::string &pcapFile) {
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eth_header;
    struct ip *ip_header;
    struct tcphdr *tcp_header;
    struct udphdr *udp_header;
    int packetCount = 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(pcapFile.c_str(), errbuf);


    while ((packet = pcap_next(handle, &header)) != nullptr) {
        packetCount++;
        eth_header = (struct ether_header *) packet;

        if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
            ip_header = (struct ip *) (packet + sizeof(struct ether_header));

            std::cout << "Packet #" << packetCount << std::endl;
            std::cout << "Time: " << header.ts.tv_sec << std::endl;
            std::cout << "Source IP: " << inet_ntoa(ip_header->ip_src) << std::endl;
            std::cout << "Destination IP: " << inet_ntoa(ip_header->ip_dst) << std::endl;
            std::cout << "Protocol: " << (unsigned int) ip_header->ip_p << std::endl;
            std::cout << "Length: " << ntohs(ip_header->ip_len) << std::endl;

            if (ip_header->ip_p == IPPROTO_TCP) {
                tcp_header = (struct tcphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                std::cout << "Source Port: " << ntohs(tcp_header->th_sport) << std::endl;
                std::cout << "Destination Port: " << ntohs(tcp_header->th_dport) << std::endl;
            } else if (ip_header->ip_p == IPPROTO_UDP) {
                udp_header = (struct udphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                std::cout << "Source Port: " << ntohs(udp_header->uh_sport) << std::endl;
                std::cout << "Destination Port: " << ntohs(udp_header->uh_dport) << std::endl;
            }

            std::cout << std::endl;
        }
    }
    pcap_close(handle);
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
    auto *reader = reinterpret_cast<PcapReader*>(userData);
    std::cout << "Packet length: " << pkthdr->len << std::endl;
    // Add more packet processing code here
}

