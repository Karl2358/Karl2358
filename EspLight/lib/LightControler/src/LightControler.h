#ifndef LIGHT_CONTROLER_H
#define LIGHT_CONTROLER_H
#include "LightModel.h"
#include "PwmControler.h"
#include <map>
#include  "StorageControler.h"
#include <Arduino.h>

namespace lightControler{
    struct hsb {
        float h;
        float s;
        float b;
    };
    class LightControler
    {
    private:
        /* data */
        pwm::PwmController* pwmControler;
        std::map<String,lightModel::LightModel*> lightList;
        void hsbToRGBW(hsb _hsb,int* valueList);
        void hsbToRGB(hsb _hsb, int* valueList);
        hsb rgbwToHSB(int* valueSet);
        hsb rgbToHSB(int* valueSet);
        String module = "LightControler Module";
        logger::Logger* _logger;
        store::StorageControler* storeC;
        lightModel::LightModel* getLight(String lightName);
    public:
        LightControler(pwm::PwmController* _pwmControler);
        ~LightControler();
        void addLight(String light,int* channelList,lightModel::lightType type);
        void changeLight(String light,int* channelList,lightModel::lightType type);
        // void changeLight(String light,int* channelList);
        // void changeLight(String light,lightModel::lightType type);
        void removeLight(String light);
        void setLight(String light,hsb _hsb,int status);
        void setLight(String light,int status);
        hsb getHsb(String light);
        hsb getHsbOld(String light);
        int getStatus(String light);
        void setPwmControler(pwm::PwmController* _pwmControler);
        void storeLight(String lightName,String _file);
        void loadLights(String _file);
        std::map<String,lightModel::LightModel*>* getLights();
    };
    
    
    
}

#endif