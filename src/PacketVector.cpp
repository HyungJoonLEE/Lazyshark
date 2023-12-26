#include "PacketVector.h"


const std::vector<CustomPacket *> PacketVector::getPacketVector() {
    return m_packetVector;
}


void PacketVector::pushPacket(CustomPacket* cp) {
    m_packetVector.push_back(cp);
}


PacketVector::PacketVector() {
    m_packetVector.reserve(DEFAULT_SIZE);
}


const unsigned int PacketVector::getVectorSize(const std::vector<CustomPacket *> &pv) {
    return m_packetVector.size();
}


PacketVector::~PacketVector() {
    std::vector<CustomPacket*>::iterator ptr;
    for (ptr = m_packetVector.begin(); ptr < m_packetVector.end(); ptr++) {
        static_cast<CustomPacket*>(*ptr)->clear();
        delete *ptr;
    }
}
