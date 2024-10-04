# Advanced Packet Capture and Analysis Tool

## Project Overview
This project involves the development of a user-friendly tool for network packet analysis and intrusion detection. The tool enhances the visual presentation of packet data (similar to Wireshark) by reading `.pcap` files and highlighting potential security issues based on Snort rules. It simplifies packet analysis with an intuitive drag-and-drop interface and color-coded risk indicators.

For a more detailed explanation of the project, including its background, design, implementation details, and future enhancements, please refer to **report.pdf** located in this repository.

## Features
- **Packet Analysis**: Reads and processes `.pcap` files, displaying packet data with threat level indicators.
- **Snort Integration**: Uses Snort 3 for intrusion detection, highlighting suspicious network activities.
- **Real-time Capture**: Option for live packet analysis as packets are captured.
- **User-friendly Interface**: Drag-and-drop functionality and visual alerts for easier packet classification and threat detection.

## Installation

### Prerequisites
- CMake
- Qt6
- Snort 3
- Additional dependencies (refer to `report.pdf` for detailed instructions on setting up the environment)

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/HyungJoonLEE/Lazyshark.git
   cd Lazyshark

2. Create a build directory and configure the project using CMake:
   ```bash
   cmake -S . -B cmake-build-debug

3. Build the project:
   ```bash
   cmake --build cmake-build-debug
   
4. Run the application:
   ```bash
   cd cmake-build-debug
   sudo ./Lazyshark

For more detailed installation steps, including additional dependencies and troubleshooting, please refer to **report.pdf**.
