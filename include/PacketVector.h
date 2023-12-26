#ifndef LAZYSHARK_PACKETVECTOR_H
#define LAZYSHARK_PACKETVECTOR_H

#define DEFAULT_SIZE 10000

#include <vector>
#include "CustomPacket.h"

class PacketVector {
    friend class AnalyzeWindow;
public:
    PacketVector();
    ~PacketVector();
    const std::vector<CustomPacket*> getPacketVector();
    const unsigned int getVectorSize(const std::vector<CustomPacket*>& pv);
    void pushPacket(CustomPacket* cp);
private:
    std::vector<CustomPacket*> m_packetVector;
    unsigned int vectorSize;
};


#endif //LAZYSHARK_PACKETVECTOR_H
