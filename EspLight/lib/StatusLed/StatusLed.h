#ifndef STATUSLED
#define STATUSLED
#include "TaskSchedulerDeclarations.h"
#include "Arduino.h"
#include "Logger.h"

namespace status
{
    


    class StatusLed
    {
    private:
        /* data */
        static StatusLed* statusLedIntance;
        StatusLed(Scheduler* sched);
        ~StatusLed();
        Task blinkTask;
        int ledPin = 2;
        boolean state = false;
        void ledOn(){digitalWrite(ledPin,LOW);
                    state=true;};
        void ledOff(){digitalWrite(ledPin,HIGH);
                    state = false;};
        void run();
        int offTime;
        int onTime;
        logger::Logger* _logger; 
        
    public:
        static StatusLed* getSatusLed(Scheduler* sched);
        static StatusLed* getSatusLed();
        void set(int _onTime,int _offTime){
            offTime = _offTime;
            onTime = _onTime;
        };
    };


} // namespace status

#endif