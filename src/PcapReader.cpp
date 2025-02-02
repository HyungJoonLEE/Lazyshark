#include "PcapReader.h"


PcapReader::PcapReader() :  descr(nullptr) {
    _pv.reserve(10000);
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


void PcapReader::readPcapFile(const string &pcapFile,
                              vector<CustomPacket *> &av,
                              const unordered_map<string, tuple<int, string>> &logMap) {
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eth_hdr;
    struct ip *ipv4;
    struct ip6_hdr *ipv6;
    struct tcphdr *tcp_hdr;
    struct udphdr *udp_hdr;
    struct icmphdr *icmp_hdr;
    const char *payload;
    unsigned int packetCount = 0;
    unsigned int payloadLen = 0;


    av.reserve(30000);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(pcapFile.c_str(), errbuf);

    while ((packet = pcap_next(handle, &header)) != nullptr) {
        auto *cp = new CustomPacket;
        packetCount++;

        string time = formatTime(header);
        string logTime = removeYear(time);

        cp->setNo(packetCount);
        cp->setLen(header.len);
        cp->setTime(time);
        cp->setWarning(logTime, logMap);

        eth_hdr = (struct ether_header *) packet;

        switch (ntohs(eth_hdr->ether_type)) {
            case ETHERTYPE_ARP:
                cp->processARP(eth_hdr);
                break;
            case ETHERTYPE_IP:
                ipv4 = (struct ip *) (packet + sizeof(struct ether_header));
                cp->processIP(ipv4, "ipv4");
                switch (ipv4->ip_p) {
                    case IPPROTO_TCP:
                        tcp_hdr = (struct tcphdr *) (packet + SIZE_ETH + SIZE_IPV4);
                        payload = (char *) (packet + SIZE_ETH + SIZE_IPV4 + SIZE_TCP);
                        payloadLen = ntohs(ipv4->ip_len) - ipv4->ip_hl * 4 - SIZE_TCP;
                        cp->processTCP(tcp_hdr);
                        break;
                    case IPPROTO_UDP:
                        udp_hdr = (struct udphdr *) (packet + SIZE_ETH + SIZE_IPV4);
                        payload = (char *) (packet + SIZE_ETH + SIZE_IPV4 + SIZE_UDP);
                        payloadLen = ntohs(ipv4->ip_len) - ipv4->ip_hl * 4 - SIZE_UDP;
                        cp->processUDP(udp_hdr);
                        break;
                    case IPPROTO_ICMP:
                        icmp_hdr = (struct icmphdr *) (packet + SIZE_ETH + SIZE_IPV4);
                        cp->processICMP(icmp_hdr);
                        // TODO: implement ICMP
                        break;
                    default:
                        cout << "Shouldn't be here: " << ipv4->ip_p << endl;
                        break;
                }
                break;
            case (uint16_t)ETHERTYPE_IPV6:
                ipv6 = (struct ip6_hdr *) (packet + sizeof(struct ether_header));
                cp->processIP(ipv6, "ipv6");
                int next_header = ipv6->ip6_nxt;
                int offset = SIZE_ETH + SIZE_IPV6;
                int totalPayloadLength = ntohs(ipv6->ip6_plen);
                switch (next_header) {
                    case IPPROTO_TCP:
                        tcp_hdr = (struct tcphdr *)(packet + offset);
                        cp->processTCP(tcp_hdr);
                        // Calculate payload length if necessary
                        payloadLen = totalPayloadLength - ((tcp_hdr->th_off << 2) + offset - SIZE_ETH);
                        break;
                    case IPPROTO_UDP:
                        udp_hdr = (struct udphdr *)(packet + offset);
                        cp->processUDP(udp_hdr);
                        // Calculate payload length if necessary
                        payloadLen = totalPayloadLength - (SIZE_UDP + offset - SIZE_ETH);
                        break;
                    case IPPROTO_ICMPV6:
                        cp->processICMPV6();
                        break;
                    default:
                        cout << "Unhandled IPv6 Next Header: " << next_header << endl;
                        break;
                }
                break;
        }
//        if (!cp->getWarning().empty()) {
            cp->savePayload(packet, header.len);
//        }
        av.push_back(cp);
    }

    pcap_close(handle);
}


void PcapReader::close() {
    if (descr != nullptr) {
        pcap_close(descr);
        descr = nullptr;
    }
}


string PcapReader::formatTime(const pcap_pkthdr &header) {
    stringstream ss;
    char timestamp[64] = {0};
    time_t timeSec = header.ts.tv_sec;
    struct tm *timeInfo = gmtime(&timeSec);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d-%H:%M:%S", timeInfo);
    ss << timestamp << "." << setfill('0') << setw(6) << header.ts.tv_usec;
    return ss.str();
}


string PcapReader::removeYear(const string &time) {
    std::string year = time.substr(0, 4);
    std::string month = time.substr(5, 2);
    std::string day = time.substr(8, 2);
    std::string rest = time.substr(10); // This gets " HH:MM:SS.ffffff"

    // Construct the new timestamp
    std::stringstream UTC_Time;
    UTC_Time << month << "/" << day << rest; // "MM/DD-HH:MM:SS.ffffff"

    return UTC_Time.str();
}


