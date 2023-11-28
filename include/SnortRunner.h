#ifndef LAZYSHARK_SNORTRUNNER_H
#define LAZYSHARK_SNORTRUNNER_H

#include <string>
#include <cstdlib>
#include <memory>
#include <iostream>


class SnortRunner {
public:
    SnortRunner();
    SnortRunner(const SnortRunner& ref);
    ~SnortRunner();
    SnortRunner& operator= (const SnortRunner& ref) {}

    void generateSnortLog(std::string &pcapFile);
    static std::string extractFilename(const std::string &filepath);

    static SnortRunner& getInstance() {
        static SnortRunner snortRunner;
        return snortRunner;
    }

private:

};
#endif //LAZYSHARK_SNORTRUNNER_H
