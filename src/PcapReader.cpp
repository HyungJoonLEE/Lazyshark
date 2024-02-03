#include "PcapReader.h"


PcapReader::PcapReader() :  descr(nullptr) {
    packetVector.reserve(10000);
}


PcapReader::~PcapReader() {
    close();
}


bool PcapReader::open(const string &pcapFile) {
    char errbuf[PCAP_ERRBUF_SIZE];
    descr = pcap_open_offline(pcapFile.c_str(), errbuf);
    if (descr == nullptr) {
        cerr << "pcap_open_offline() failed: " << errbuf << endl;
        return false;
    }
    return true;
}


void PcapReader::readPcap(const string &pcapFile) {
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
        cout << "Packet #" << cp->getNo() << endl;

        eth_header = (struct ether_header *) packet;
        cp->setEthHdr(eth_header);


        if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
            ip_header = (struct ip *) (packet + sizeof(struct ether_header));
            cp->setIpv4Hdr(ip_header);

            cout << "Time: " << header.ts.tv_sec << endl;
            cout << "Source IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_src) << endl;
            cout << "Destination IP: " << inet_ntoa(cp->getIpv4Hdr()->ip_dst) << endl;
            cout << "Protocol: " << (unsigned short) cp->getIpv4Hdr()->ip_p << endl;
            cout << "Length: " << cp->getIpv4Hdr()->ip_hl * 4 << endl;
            cout << "Total Length: " << (short) ntohs(cp->getIpv4Hdr()->ip_len) << endl;

            if (ip_header->ip_p == IPPROTO_TCP) {
                tcp_header = (struct tcphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                payload = (char *) (packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
                payload_size = ntohs(cp->getIpv4Hdr()->ip_len) - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct tcphdr);

                cp->setTCPHdr(tcp_header);
                cout << "Source Port: " << ntohs(cp->getTCPHdr()->th_sport) << endl;
                cout << "Destination Port: " << ntohs(cp->getTCPHdr()->th_dport) << endl;

                if (payload_size > 0) {
                    printf("    Payload (%d bytes):\n", payload_size);
                    cp->printPayload(reinterpret_cast<const u_char *>(payload), payload_size);
                }
            }
            else if (ip_header->ip_p == IPPROTO_UDP) {
                udp_header = (struct udphdr *) (packet + sizeof(struct ether_header) + sizeof(struct ip));
                payload_size = cp->getIpv4Hdr()->ip_len - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct udphdr);

                cp->setUDPHdr(udp_header);

                cout << "Source Port: " << ntohs(cp->getUDPHdr()->uh_sport) << endl;
                cout << "Destination Port: " << ntohs(cp->getUDPHdr()->uh_dport) << endl;
            }

            cout << endl;
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
    cout << "Packet length: " << pkthdr->len << endl;
    // Add more packet processing code here
}

