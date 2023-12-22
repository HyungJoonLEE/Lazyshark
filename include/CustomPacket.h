#ifndef LAZYSHARK_PACKET_H
#define LAZYSHARK_PACKET_H

#include <vector>
#include <string>

class CustomPacket {
public:
    CustomPacket();
    ~CustomPacket();
private:
    unsigned int packet_num;
    std::string protocol;
    std::string source_ip;
    std::string destination_ip;
    unsigned short source_port;
    unsigned short destination_port;
    std::string data;
    std::string color;
};

#endif //LAZYSHARK_PACKET_H
