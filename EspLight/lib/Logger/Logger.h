#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <FS.h>
namespace logger{
    class Logger{
        public:
            static Logger* Intance();
            void logDebug(String module, String msg);
            void logInfo(String module, String msg);

        private:
            Logger();
            static Logger* m_pInstance;
            void logToFile(String module, String msg);
            File f;
            String timeStamp(void);

    };
}
#endif