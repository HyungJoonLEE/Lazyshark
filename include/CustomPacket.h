#ifndef LAZYSHARK_PACKET_H
#define LAZYSHARK_PACKET_H

#include <vector>
#include <cstring>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <cstring>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;
enum class Color { RED, ORANGE, YELLOW, WHITE };

class CustomPacket {
public:
    void setNo(unsigned int packetCount);
    void setTime(const string &time);
    void setLen(unsigned int plen);
    void setEthHdr(const struct ether_header* hdr);
    void setIpv4Hdr(const struct ip* hdr);
    void setIpv6Hdr(const struct ip6_hdr* hdr);
    void setTCPHdr(const struct tcphdr* hdr);
    void setUDPHdr(const struct udphdr* hdr);
    void setData(const char* data);
    void printPayload(const u_char *payload, size_t len);
    void print_hex_ascii_line (const u_char *payload, int len, int offset);

    unsigned int getNo() const;
    unsigned int getLen() const;
    string getTime() const;
    struct ether_header* getEthHdr() const;
    struct ip* getIpv4Hdr() const;
    struct ip6_hdr* getIpv6Hdr() const;
    struct tcphdr* getTCPHdr() const;
    struct udphdr* getUDPHdr() const;

    ~CustomPacket();

private:
    unsigned int* no_;
    string time_;
    unsigned int* len_;
    struct ether_header* eth_;
    struct ip* ipv4_;
    struct ip6_hdr* ipv6_;
    struct tcphdr* tcp_;
    struct udphdr* udp_;
    char* data_;
    char* warning_;
    Color* color_;

    template <typename T>
    void allocateAndCopy(T** dest, const T* src, size_t size) {
        if (*dest) {
            delete *dest;
        }
        *dest = new T;
        memcpy(*dest, src, size);
    }
};

#endif //LAZYSHARK_PACKET_H
