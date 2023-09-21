#include "StatusLed.h"
// #define debugStatus

using namespace status;

StatusLed* StatusLed::statusLedIntance = NULL;

StatusLed* StatusLed::getSatusLed(Scheduler* sched){

    if(!statusLedIntance){
        statusLedIntance = new StatusLed(sched);
    }

    return statusLedIntance;
}
StatusLed* StatusLed::getSatusLed(){
    return statusLedIntance;
}


StatusLed::StatusLed(Scheduler* sched)
{
    _logger = logger::Logger::Intance();
    _logger->logDebug("Status LED","Init");
    pinMode(ledPin,OUTPUT);
    blinkTask.set(10*TASK_SECOND,TASK_FOREVER,[this](){
            // _logger->logDebug(modul,"runTask");
            run();
        });
    sched->addTask(blinkTask);
    blinkTask.enable();
    
    

}

StatusLed::~StatusLed()
{
}

void StatusLed::run(){
    #ifdef debugStatus
        Serial.print("blink ");
        Serial.println(state);
    #endif
    if(state){
        _logger->logDebug("Staus Led", "LED off");
        ledOff();
        blinkTask.setInterval(offTime*TASK_SECOND);
    }else{
        _logger->logDebug("Staus Led", "LED on");
        ledOn();
        blinkTask.setInterval(onTime*TASK_SECOND);
    }

}