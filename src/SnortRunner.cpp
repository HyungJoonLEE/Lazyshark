#include "SnortRunner.h"


SnortRunner::SnortRunner() {}


SnortRunner::~SnortRunner() {}


void SnortRunner::generateSnortLog(const std::string &pcapFile) const {
    std::string pcap = extractFilename(pcapFile);
    std::string cmd = "snort -c ./../luas/snort.lua "
                      "-R ./../rules/community.rules "
                      "-r '" + pcapFile + "' -q "
                                          "-A alert_fast "
                                          "-z 4 "
                                          ">> ./../logs/"
                                          "'" + pcap + "'.log ";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Failed to open pipe\n";
        return;
    }
}


std::string SnortRunner::extractFilename(const std::string &filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    size_t lastDot = filepath.find_last_of('.');
    std::string filename = filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);

    return filename;
}
