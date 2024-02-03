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


enum class Color { RED, ORANGE, YELLOW, WHITE };

using namespace std;

class CustomPacket {
public:
    void setNo(unsigned int);
    void setTime(time_t rawTime);
    void setEthHdr(const struct ether_header* hdr);
    void setIpv4Hdr(const struct ip* hdr);
    void setIpv6Hdr(const struct ip6_hdr* hdr);
    void setTCPHdr(const struct tcphdr* hdr);
    void setUDPHdr(const struct udphdr* hdr);
    void setData(const char* data);
    void printPayload(const u_char *payload, size_t len);
    void print_hex_ascii_line (const u_char *payload, int len, int offset);
    void clear();

    const unsigned int getNo();
    const std::string* getTime();
    const struct ether_header* getEthHdr();
    const struct ip* getIpv4Hdr();
    const struct ip6_hdr* getIpv6Hdr();
    const struct tcphdr* getTCPHdr();
    const struct udphdr* getUDPHdr();

    ~CustomPacket();

private:
    unsigned int No;
    string* m_time;
    struct ether_header* m_ether;
    struct ip* m_ipv4;
    struct ip6_hdr* m_ipv6;
    struct tcphdr* m_tcp;
    struct udphdr* m_udp;
    char* m_data;
    char* m_warning;
    Color* m_color;

    template <typename T>
    void allocateAndCopy(T** dest, const T* src, size_t size) {
        if (*dest) {
            delete *dest;
        }
        *dest = new T;
        std::memcpy(*dest, src, size);
    }
};

#endif //LAZYSHARK_PACKET_H
