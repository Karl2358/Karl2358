#ifndef PWM_CONTROLER_H
#define PWM_CONTROLER_H
#include <Adafruit_PWMServoDriver.h>
#include "Logger.h"
#include <Arduino.h>

namespace pwm
{
    class PwmController{
        public:
            PwmController();
            void setChannel(int channel,int value); 
        private: 
            Adafruit_PWMServoDriver pwmC; 
            logger::Logger* _logger; 
            String modul = "PWM Module";   

    };
} // namespace pwm

#endif