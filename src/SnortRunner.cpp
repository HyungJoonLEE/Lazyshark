#include "SnortRunner.h"


SnortRunner::SnortRunner() {}


SnortRunner::~SnortRunner() {}


void SnortRunner::generateSnortLog(const string &pcapFile) const {
    string pcap = extractFilename(pcapFile);
    string cmd = "snort -c ./../luas/snort.lua "
                      "-R ./../rules/community.rules "
                      "-r '" + pcapFile + "' -q "
                                          "-A alert_fast "
                                          "-z 4 "
                                          ">> ./../logs/"
                                          "'" + pcap + "'.log ";
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        cerr << "Failed to open pipe\n";
        return;
    }
}


string SnortRunner::extractFilename(const string &filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    size_t lastDot = filepath.find_last_of('.');
    string filename = filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);

    return filename;
}
