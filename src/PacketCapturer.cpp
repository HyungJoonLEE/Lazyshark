#include "PacketCapturer.h"
#include <QDebug>
#include <iostream>

PacketCapturer::PacketCapturer(QObject *parent) : QObject(parent) {
}

void PacketCapturer::startCapture() {
    // Setup and start packet capturing here (e.g., pcap_loop)
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *temp;
    int i = 0;

    // Find all available devices
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        qDebug() << "Error in pcap_findalldevs: " << errbuf;
        return;
    }

    // Just example: use the first available device
    if (interfaces) {
        qDebug() << "Using device: " << interfaces->name;
        pcap_t *handle = pcap_open_live(interfaces->name, BUFSIZ, 1, -1, errbuf);

        if (handle == nullptr) {
            qDebug() << "Could not open device: " << errbuf;
        } else {
            // Start packet capturing loop
            pcap_loop(handle, 0, packetHandler, nullptr);
            pcap_close(handle);
        }
    }

    pcap_freealldevs(interfaces);

//    // For each captured packet, emit newPacket signal
//    emit newPacket("Captured packet info");
}


void packetHandler(u_char *userData, const struct pcap_pkthdr *packetHeader, const u_char *packetData) {
    // Increment the packet counter
    int *packetCount = reinterpret_cast<int*>(userData);
    (*packetCount)++;

    // Print packet number and length
    std::cout << "Packet #" << *packetCount << ": ";
    std::cout << packetHeader->len << " bytes" << std::endl;

    // You could add more detailed packet processing here
}