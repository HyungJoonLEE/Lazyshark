#include "CustomPacket.h"


CustomPacket::~CustomPacket() {
    if (!m_time)    delete m_time;
    if (!m_ether)   delete m_ether;
    if (!m_ipv4)    delete m_ipv4;
    if (!m_ipv6)    delete m_ipv6;
    if (!m_tcp)     delete m_tcp;
    if (!m_udp)     delete m_udp;
    if (!m_data)     delete m_data;
    if (!m_warning) delete m_warning;
    if (!m_color)   delete m_color;
}

const unsigned int CustomPacket::getNo() { return No; }
const string* CustomPacket::getTime() { return m_time; }
const struct ether_header* CustomPacket::getEthHdr() { return m_ether; }
const struct ip* CustomPacket::getIpv4Hdr() { return m_ipv4; }
const struct ip6_hdr* CustomPacket::getIpv6Hdr() { return m_ipv6; }
const struct tcphdr* CustomPacket::getTCPHdr() { return m_tcp; }
const struct udphdr* CustomPacket::getUDPHdr() { return m_udp; }


void CustomPacket::setNo(unsigned int i) {
    this->No = i;
}


void CustomPacket::setTime(time_t rawTime) {
    std::tm* timeInfo = std::localtime(&rawTime);
    std::stringstream ss;
    ss << std::put_time(timeInfo, "%Y/%m/%d-%H:%M:%S");
    m_time = new std::string(ss.str());
}


void CustomPacket::setEthHdr(const struct ether_header *hdr) {
    allocateAndCopy(&m_ether, hdr, sizeof(struct ether_header));
}


void CustomPacket::setIpv4Hdr(const struct ip *hdr) {
    allocateAndCopy(&m_ipv4, hdr, sizeof(struct ip));
}


void CustomPacket::setIpv6Hdr(const struct ip6_hdr *hdr) {
    allocateAndCopy(&m_ipv6, hdr, sizeof(struct ip6_hdr));
}


void CustomPacket::setTCPHdr(const struct tcphdr *hdr) {
    allocateAndCopy(&m_tcp, hdr, sizeof(struct tcphdr));
}


void CustomPacket::setUDPHdr(const struct udphdr *hdr) {
    allocateAndCopy(&m_udp, hdr, sizeof(struct udphdr));
}


void CustomPacket::setData(const char *data) {
    allocateAndCopy(&m_data, data, sizeof(struct tcphdr));
}


void CustomPacket::printPayload(const u_char *payload, size_t len) {
    int len_rem = len;
    int line_width = 16;		// number of bytes per line
    int line_len;
    int offset = 0;			// offset counter
    const u_char *ch = payload;

    if (len <= 0)
        return;

    // does data fits on one line?
    if (len <= line_width) {
        print_hex_ascii_line (ch, len, offset);
        return;
    }

    // data spans multiple lines
    for ( ;; ) {
        // determine the line length and print
        line_len = line_width % len_rem;
        print_hex_ascii_line (ch, line_len, offset);

        // Process the remainder of the line
        len_rem -= line_len;
        ch += line_len;
        offset += line_width;

        // Ensure we have line width chars or less
        if (len_rem <= line_width) {
            //print last line
            print_hex_ascii_line (ch, len_rem, offset);
            break;
        }
    }
}


void CustomPacket::print_hex_ascii_line(const u_char *payload, int len, int offset) {
    int i;
    int gap;
    const u_char *ch;
    char temp[1024] = {0};

    // the offset
    printf("    %05d   ", offset);


    // print in hex
    ch = payload;
    for (i = 0; i < len; i++) {
        printf("%02x ", *ch);
        sprintf(temp + strlen(temp), "%02x ", *ch);
        ch++;
        if (i == 7) {
            printf(" ");
            sprintf(temp + strlen(temp), " ");
        }
    }


    // print spaces to handle a line size of less than 8 bytes
    if (len < 8) {
        printf(" ");
    }


    // Pad the line with whitespace if necessary
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) printf("   ");
    }

    printf("   ");


    // Print ASCII
    ch = payload;

    for (i = 0; i < len; i++) {
        if (isprint(*ch)) {
            printf("%c", *ch);
            sprintf(temp + strlen(temp), "%c", *ch);
        }
        else {
            printf(".");
            sprintf(temp + strlen(temp), ".");
        }
        ch++;
    }
    printf("\n");
    sprintf(temp + strlen(temp), "\n");
}

void CustomPacket::clear() {
    if (!m_time) delete m_time;
    if (!m_ether) delete m_ether;
    if (!m_ipv4) delete m_ipv4;
    if (!m_ipv6) delete m_ipv6;
    if (!m_tcp) delete m_tcp;
    if (!m_udp) delete m_udp;
    if (!m_data) delete m_data;
    if (!m_warning) delete m_warning;
    if (!m_color) delete m_color;
}

