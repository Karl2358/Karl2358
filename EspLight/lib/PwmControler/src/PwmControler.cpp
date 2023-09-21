#include "PwmControler.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
using namespace pwm;


PwmController::PwmController(){
    _logger = logger::Logger::Intance();
    pwmC = Adafruit_PWMServoDriver();
    pwmC.begin();
    pwmC.setPWMFreq(1600);
};

void PwmController::setChannel(int channel, int value){
    if(channel>=0 && channel<=15 && value>=0 && value <=4095){
        pwmC.setPWM(channel,0,value);
        _logger->logInfo(modul,"Set Channel "+String(channel)+" to Value "+String(value));
    }else{
        _logger->logInfo(modul,"out of bound request for Ch "+String(channel)+" value: "+String(value));
    }
}



