#include "CustomPacket.h"


CustomPacket::~CustomPacket() {
    if (!no_)           delete no_;
    if (!eth_)          delete eth_;
    if (!ipv4_)         delete ipv4_;
    if (!ipv6_)         delete ipv6_;
    if (!tcp_)          delete tcp_;
    if (!udp_)          delete udp_;
    if (!data_)         delete data_;
    if (!warning_)      delete warning_;
    if (!color_)        delete color_;
}


unsigned int CustomPacket::getNo() const { return *no_; }
unsigned int CustomPacket::getLen() const { return *len_; }
string CustomPacket::getTime() const { return time_; }
struct ether_header *CustomPacket::getEthHdr() const { return eth_; }
struct ip *CustomPacket::getIpv4Hdr() const { return ipv4_; }
struct ip6_hdr *CustomPacket::getIpv6Hdr() const { return ipv6_; }
struct tcphdr *CustomPacket::getTCPHdr() const { return tcp_; }
struct udphdr *CustomPacket::getUDPHdr() const { return udp_; }


void CustomPacket::setNo(const unsigned int packetCount) {
    no_ = new unsigned int;
    *no_ = packetCount;
}


void CustomPacket::setTime(const string& time) {
    time_ = time;
}


void CustomPacket::setLen(const unsigned int len) {
    len_ = new unsigned int;
    *len_ = len;
}


void CustomPacket::setEthHdr(const struct ether_header *hdr) {
    allocateAndCopy(&eth_, hdr, sizeof(struct ether_header));
}


void CustomPacket::setIpv4Hdr(const struct ip *hdr) {
    allocateAndCopy(&ipv4_, hdr, sizeof(struct ip));
}


void CustomPacket::setIpv6Hdr(const struct ip6_hdr *hdr) {
    allocateAndCopy(&ipv6_, hdr, sizeof(struct ip6_hdr));
}


void CustomPacket::setTCPHdr(const struct tcphdr *hdr) {
    allocateAndCopy(&tcp_, hdr, sizeof(struct tcphdr));
}


void CustomPacket::setUDPHdr(const struct udphdr *hdr) {
    allocateAndCopy(&udp_, hdr, sizeof(struct udphdr));
}


void CustomPacket::setData(const char *data) {
    allocateAndCopy(&data_, data, sizeof(struct tcphdr));
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







