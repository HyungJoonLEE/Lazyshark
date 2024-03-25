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


void CustomPacket::processICMPV6() {
    if (protocol_.empty()) protocol_ = "ICMPV6";
}


void CustomPacket::savePayload(const u_char *payload, size_t len) {
    int len_rem = len;
    int line_width = 16;		// number of bytes per line
    int line_len;
    int offset = 0;			// offset counter
    const u_char *ch = payload;
    std::ostringstream oss;


    if (len <= 0)
        return;

    if (len <= line_width) {
        print_hex_ascii_line (ch, len, offset, oss);
        return;
    }

    for ( ;; ) {
        // determine the line length and print
        line_len = line_width % len_rem;
        print_hex_ascii_line (ch, line_len, offset, oss);

        // Process the remainder of the line
        len_rem -= line_len;
        ch += line_len;
        offset += line_width;

        // Ensure we have line width chars or less
        if (len_rem <= line_width) {
            //print last line
            print_hex_ascii_line (ch, len_rem, offset, oss);
            break;
        }
    }
}

void CustomPacket::print_hex_ascii_line(const u_char *payload, int len, int offset, ostringstream &oss) {
    int i;
    int gap;
    const u_char *ch;

    // Append the offset in hexadecimal
    oss << std::setw(5) << std::setfill('0') << std::hex << offset << "   ";

    // Append the hex values
    ch = payload;
    for (i = 0; i < len; i++) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*ch) << " ";
        ch++;
        // Append extra space after 8th byte for visual aid
        if (i == 7)
            oss << " ";
    }
    // Append space to handle line less than 8 bytes
    if (len < 8)
        oss << " ";

    // Fill line with whitespace if not the full width
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            oss << "   ";
        }
    }
    oss << "   ";

    // Append the ASCII values
    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            oss << *ch;
        else
            oss << ".";
        ch++;
    }

    oss << "\n";
    data_ = oss.str();
}

