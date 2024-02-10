#ifndef LAZYSHARK_SNORTRUNNER_H
#define LAZYSHARK_SNORTRUNNER_H

#include <string>
#include <cstdlib>
#include <memory>
#include <iostream>

using namespace std;

class SnortRunner {
public:
    SnortRunner();
    SnortRunner(const SnortRunner& ref);
    ~SnortRunner();
    SnortRunner& operator= (const SnortRunner& ref) {}

    void generateSnortLog(const string &pcapFile) const;
    static string extractFilename(const string &filepath);

    static SnortRunner& getInstance() {
        static SnortRunner snortRunner;
        return snortRunner;
    }

private:

};
#endif //LAZYSHARK_SNORTRUNNER_H
