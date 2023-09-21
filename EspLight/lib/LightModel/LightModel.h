#ifndef LIGHT_MODEL_H
#define LIGHT_MODEL_H
#include <Arduino.h>
#include "Logger.h"

namespace lightModel
{
    enum lightType {
        rgbw,
        rgb
    };
    class LightModel
    {
    private:
        lightType type;
        int* channelList;
        int* valueList;
        int* lastValueList;
        int status;
        int lastStatus;
        String module ="LightModel modul"; 
        logger::Logger* _logger;
    public:
        LightModel(lightType type, int* _channelList);
        ~LightModel();
        void set(int status);
        void set(int status,int* valueList);
        int getStatus(){return status;};
        int getLastStatus(){return lastStatus;};
        int* getValue(){return valueList;};
        int* getLastvalue(){return lastValueList;};
        int* getChannels(){return channelList;};
        lightType getType(){return type;};
    };
    
    
    
    
} // namespace lightModel

#endif