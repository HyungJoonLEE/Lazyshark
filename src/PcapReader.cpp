#include "PcapReader.h"


PcapReader::PcapReader() :  descr(nullptr) {
    packetVector.reserve(10000);
}


PcapReader::~PcapReader() {
    close();
}


bool PcapReader::open(const std::string &pcapFile) {
    char errbuf[PCAP_ERRBUF_SIZE];
    descr = pcap_open_offline(pcapFile.c_str(), errbuf);
    if (descr == nullptr) {
        std::cerr << "pcap_open_offline() failed: " << errbuf << std::endl;
        return false;
    }
    return true;
}


void PcapReader::readPcap(const std::string &pcapFile) {
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eth_header;
    struct ip *ip_header;
    struct tcphdr *tcp_header;
    struct udphdr *udp_header;
    const char *payload;
    unsigned int packetCount = 0;
    unsigned int payload_size = 0;


    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(pcapFile.c_str(), errbuf);


    while ((packet = pcap_next(handle, &header)) != nullptr) {
        auto* cp = new CustomPacket();
        packetCount++;

        cp->setNo(packetCount);
        std::cout << "Packet #" << cp->getNo() << std::endl;

        eth_header = (struct ether_header *) packet;
        cp->setEthHdr(eth_header);


        if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
            ip_header = (struct ip *) (packet + sizeof(struct ether_header));
            cp->setIpv4Hdr(ip_header);

            std::cout << "Time: " << header.ts.tv_sec << std::endl;
            std::cout << "Source IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_src) << std::endl;
            std::cout << "Destination IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_dst) << std::endl;
            std::cout << "Protocol: " << (unsigned short) cp->getIpv4Hdr()->ip_p << std::endl;
            std::cout << "Length: " << cp->getIpv4Hdr()->ip_hl * 4 << std::endl;
            std::cout << "Total Length: " << (short) ntohs(cp->getIpv4Hdr()->ip_len) << std::endl;

            if (ip_header->ip_p == IPPROTO_TCP) {
                tcp_header = (struct tcphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                payload = (char *) (packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
                payload_size = ntohs(cp->getIpv4Hdr()->ip_len) - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct tcphdr);

                cp->setTCPHdr(tcp_header);
                std::cout << "Source Port: " << ntohs(cp->getTCPHdr()->th_sport) << std::endl;
                std::cout << "Destination Port: " << ntohs(cp->getTCPHdr()->th_dport) << std::endl;

                if (payload_size > 0) {
                    printf("    Payload (%d bytes):\n", payload_size);
                    cp->printPayload(reinterpret_cast<const u_char *>(payload), payload_size);
                }
            }
            else if (ip_header->ip_p == IPPROTO_UDP) {
                udp_header = (struct udphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                payload_size = cp->getIpv4Hdr()->ip_len - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct udphdr);

                cp->setUDPHdr(udp_header);

                std::cout << "Source Port: " << ntohs(cp->getUDPHdr()->uh_sport) << std::endl;
                std::cout << "Destination Port: " << ntohs(cp->getUDPHdr()->uh_dport) << std::endl;
            }

            std::cout << std::endl;
            packetVector.push_back(cp);
        }
        delete cp;
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

