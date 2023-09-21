#ifndef OTA_H
#define OTA_H
#include "Logger.h"

namespace ota{
class otaRunner
{
private:
    /* data */
    logger::Logger* _logger;
public:
    otaRunner(/* args */);
    ~otaRunner();
    void run();
};



}
#endif