#include "CustomPacket.h"


unordered_map<int, string> portMap;


unsigned int CustomPacket::getNo() const { return no_; }
unsigned int CustomPacket::getLen() const { return len_; }
string CustomPacket::getTime() const { return time_; }
string CustomPacket::getProtocol() const { return protocol_; }
string CustomPacket::getSIP() const { return source_; }
string CustomPacket::getDIP() const { return dest_; }
uint16_t CustomPacket::getSPort() const { return sport_; }
uint16_t CustomPacket::getDPort() const { return dport_; }
string CustomPacket::getWarning() const { return warning_; }
string CustomPacket::getData() const { return data_; }
int CustomPacket::getPriority() const { return priority_; }


void CustomPacket::setNo(const unsigned int packetCount) {
    no_ = packetCount;
}


void CustomPacket::setTime(const string& time) {
    time_ = time;
}


void CustomPacket::setLen(const unsigned int len) {
    len_ = len;
}


void CustomPacket::setData(const string &data) {
    data_ = data;
}



void CustomPacket::processIP(const void *hdr, const string &type) {
    if (type == "ipv4") {
        auto *ipv4 = (ip *) hdr;
        source_ = inet_ntoa(ipv4->ip_src);
        dest_ = inet_ntoa(ipv4->ip_dst);
    }
    else if (type == "ipv6") {
        auto *ipv6 = (ip6_hdr *) hdr;
        char saddr[INET6_ADDRSTRLEN] = {0}, daddr[INET6_ADDRSTRLEN] = {0};
        source_ = inet_ntop(AF_INET6, &(ipv6->ip6_src), saddr, INET6_ADDRSTRLEN);
        dest_ = inet_ntop(AF_INET6, &(ipv6->ip6_dst), daddr, INET6_ADDRSTRLEN);
    }
}


void CustomPacket::processTCP(const struct tcphdr *hdr) {
    sport_ = ntohs(hdr->source);
    dport_ = ntohs(hdr->dest);
    protocol_ = portMap[sport_];
    if (protocol_.empty()) protocol_ = "TCP";
}


void CustomPacket::processUDP(const struct udphdr *hdr) {
    sport_ = ntohs(hdr->source);
    dport_ = ntohs(hdr->dest);
    protocol_ = portMap[sport_];
    if (protocol_.empty()) protocol_ = "UDP";
}


void CustomPacket::processICMP(const struct icmphdr *hdr) {
    if (protocol_.empty()) protocol_ = "ICMP";
    // TODO: Need to figure out handling port
}


void CustomPacket::setWarning(const string &logTime, const unordered_map<string, tuple<int, string>> &logMap) {
    auto it = logMap.find(logTime);
    if (it != logMap.end()) {
        // Access the tuple
        auto& [priority, classification] = it->second;
        priority_ = priority;
        warning_ = classification;
    }
}


void CustomPacket::processARP(const struct ether_header *hdr) {
    if (protocol_.empty()) protocol_ = "ARP";
    sport_ = ntohs(0);
    dport_ = ntohs(0);

    char src_mac_str[18] = {0}; // 17 for MAC address in text + null terminator
    char dst_mac_str[18] = {0};

    snprintf(src_mac_str, sizeof(src_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             hdr->ether_shost[0], hdr->ether_shost[1], hdr->ether_shost[2],
             hdr->ether_shost[3], hdr->ether_shost[4], hdr->ether_shost[5]);

    snprintf(dst_mac_str, sizeof(dst_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             hdr->ether_dhost[0], hdr->ether_dhost[1], hdr->ether_dhost[2],
             hdr->ether_dhost[3], hdr->ether_dhost[4], hdr->ether_dhost[5]);

    source_ = string(src_mac_str);
    dest_ = string(dst_mac_str);

    if (source_ == "ff:ff:ff:ff:ff:ff") {
        source_ = "Broadcast";
    }
    if (dest_ == "ff:ff:ff:ff:ff:ff") {
        dest_ = "Broadcast";
    }
}

