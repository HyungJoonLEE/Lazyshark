#include "PcapReader.h"


PcapReader::PcapReader() :  descr(nullptr) {
//    _pv.reserve(10000);
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
    const char *payload;
    unsigned int packetCount = 0;
    unsigned int payloadLen = 0;
    ostringstream hexStream;


    av.reserve(10000);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(pcapFile.c_str(), errbuf);

    while ((packet = pcap_next(handle, &header)) != nullptr) {
        auto *cp = new CustomPacket;
        packetCount++;

        string time = formatTime(header);
        string logTime = removeYear(header);

        cp->setNo(packetCount);
        cp->setLen(header.len);
        cp->setTime(time);
        cout << logTime << endl;
        cp->setWarning(logTime, logMap);


        eth_hdr = (struct ether_header *) packet;

        switch (ntohs(eth_hdr->ether_type)) {
            case ETHERTYPE_IP:
                ipv4 = (struct ip *) (packet + sizeof(struct ether_header));
                cp->processIP(ipv4, "ipv4");
                switch (ipv4->ip_p) {
                    case IPPROTO_TCP:
                        tcp_hdr = (struct tcphdr *) (packet + SIZE_ETH + SIZE_IPV4);
                        payload = (char *) (packet + SIZE_ETH + SIZE_IPV4 + SIZE_TCP);
                        payloadLen = ntohs(ipv4->ip_len) - ipv4->ip_hl * 4 - SIZE_TCP;
                        cp->processTCP(tcp_hdr);
//                        if (payloadLen > 0) {
//                            for (int i = 0; i < payloadLen; ++i) {
//                                hexStream << hex << setw(2) << setfill('0') << static_cast<int>(payload[i]);
//                                if (i < payloadLen - 1) {
//                                    hexStream << " "; // Optional: Add a space between bytes
//                                }
//                            }
//                            cp->setData(hexStream.str());
//                            hexStream.clear();
//                        }
                        break;
                    case IPPROTO_UDP:
                        udp_hdr = (struct udphdr *) (packet + SIZE_ETH + SIZE_IPV4);
                        payload = (char *) (packet + SIZE_ETH + SIZE_IPV4 + SIZE_UDP);
                        payloadLen = ntohs(ipv4->ip_len) - ipv4->ip_hl * 4 - SIZE_UDP;
                        cp->processUDP(udp_hdr);
//                        if (payloadLen > 0) {
//                            for (int i = 0; i < payloadLen; ++i) {
//                                hexStream << hex << setw(2) << setfill('0') << static_cast<int>(payload[i]);
//                                if (i < payloadLen - 1) {
//                                    hexStream << " "; // Optional: Add a space between bytes
//                                }
//                            }
//                            cp->setData(hexStream.str());
//                            hexStream.clear();
//                        }
                        break;
                    case IPPROTO_ICMP:
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
                // TODO: Different compared to ipv4 (No tcp or udp protocol indicated)
//                switch (ipv6.) {
//                    case IPPROTO_TCP:
//                        tcp_hdr = (struct tcphdr *) (packet + SIZE_ETH + SIZE_IPV6);
//                        cp->processTCP(tcp_hdr);
//                        break;
//                    case IPPROTO_UDP:
//                        udp_hdr = (struct udphdr *) (packet + SIZE_ETH + SIZE_IPV6);
//                        cp->processUDP(udp_hdr);
//                        break;
//
//                    default:
//                        cout << "Shouldn't be here" << endl;
//                        break;
//                }
                break;
            case ETHERTYPE_ARP:
                break;
            default:
                break;
        }
        av.push_back(cp);
    }
    pcap_close(handle);
}



//                payload = (char *) (packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
//                payload_size = ntohs(cp->getIpv4Hdr()->ip_len) - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct tcphdr);
//                if (payload_size > 0) {
//                    printf("    Payload (%d bytes):\n", payload_size);
//                    cp->printPayload(reinterpret_cast<const u_char *>(payload), payload_size);
//                }
//
//                payload_size = cp->getIpv4Hdr()->ip_len - cp->getIpv4Hdr()->ip_hl * 4 - sizeof(struct udphdr);
//                if (payload_size > 0) {
//                    printf("    Payload (%d bytes):\n", payload_size);
//                    cp->printPayload(reinterpret_cast<const u_char *>(payload), payload_size);
//                }




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
    struct tm *timeInfo = localtime(&timeSec);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeInfo);
    ss << timestamp << "." << setfill('0') << setw(6) << header.ts.tv_usec;
    return ss.str();
}


string PcapReader::removeYear(const pcap_pkthdr &header) {
    stringstream ss;
    char timestamp[64] = {0};
    time_t timeSec = header.ts.tv_sec - 3600;
//    time_t timeSec = header.ts.tv_sec;
    struct tm *timeInfo = localtime(&timeSec);
    strftime(timestamp, sizeof(timestamp), "%m/%d-%H:%M:%S", timeInfo);
    ss << timestamp << "." << setfill('0') << setw(6) << header.ts.tv_usec;
    return ss.str();
}