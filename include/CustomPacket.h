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


enum class Color { RED, ORANGE, YELLOW, WHITE };

class CustomPacket {
public:
    void setEthHdr(const void *eth_hdr, size_t size);
    struct ether_header* getEthHdr() const;
private:
    struct ether_header* m_ether;
    struct ip* m_ip4;
    struct ip6_hdr* m_ip6;
    struct tcp_hdr* m_tcp;
    struct udp_hdr* m_udp;
    Color m_color;
};

#endif //LAZYSHARK_PACKET_H
