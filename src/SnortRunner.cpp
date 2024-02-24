#include "SnortRunner.h"


SnortRunner::SnortRunner() {}


SnortRunner::~SnortRunner() {}


void SnortRunner::generateSnortLog(const string &pcapFile) const {
    string logName = extractFilename(pcapFile);
    string cmd = "snort -c ./../luas/snort.lua "
                      "-R ./../rules/community.rules "
                      "-r '" + pcapFile + "' -q "
                                          "-A alert_fast "
                                          "-z 4 "
                                          ">> ./../logs/"
                                          "'" + logName + "'.log ";
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


void SnortRunner::processLog(const string& pcapFile) {
    string logFile = "./../logs/" + extractFilename(pcapFile) + ".log";
    ifstream file(logFile);

    string line;

    regex pattern(R"((\d{2}/\d{2}-\d{2}:\d{2}:\d{2}\.\d{6}).*\[Classification: ([^\]]+)\].*\[Priority: (\d+)\])");

    if (!file.is_open()) {
        cerr << "Failed to open file: test.log" << endl;
        exit(1);
    }

    while (getline(file, line)) {
        smatch matches;
        if (regex_search(line, matches, pattern) && matches.size() == 4) {

            // Extract time, priority, and classification from the line
            string time = matches[1].str();
            string classification = matches[2].str();
            int priority = stoi(matches[3].str());

            logMap_[time] = make_tuple(priority, classification);
        }
    }

//    for (const auto& pair : logMap_) {
//        cout << pair.first << ", "
//                  << get<0>(pair.second) << ", "
//                  << get<1>(pair.second) << endl;
//    }

    file.close();
}


unordered_map<string, tuple<int, string>> SnortRunner::getLogMap() const {
    return logMap_;
}
