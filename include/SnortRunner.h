#ifndef LAZYSHARK_SNORTRUNNER_H
#define LAZYSHARK_SNORTRUNNER_H

#include <string>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <tuple>

using namespace std;

class SnortRunner {
public:
    SnortRunner();
    SnortRunner(const SnortRunner& ref);
    ~SnortRunner();
    SnortRunner& operator= (const SnortRunner& ref) {}

    void generateSnortLog(const string &pcapFile) const;
    void processLog(const string& pcapFile);
    unordered_map<string, tuple<int, string>> getLogMap() const;

    static string extractFilename(const string &filepath);

    static SnortRunner& getInstance() {
        static SnortRunner snortRunner;
        return snortRunner;
    }

private:
    unordered_map<string, tuple<int, string>> logMap_;
};
#endif //LAZYSHARK_SNORTRUNNER_H
