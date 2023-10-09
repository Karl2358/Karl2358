#include "LightControler.h"
#include "math.h"
#include "Logger.h"
#include <ArduinoJson.h>
//#define debug
using namespace lightControler;



LightControler::LightControler(pwm::PwmController* _pwmControler):pwmControler(_pwmControler)
    {
        _logger = logger::Logger::Intance();
        _logger->logInfo(module,"Initailized");
        storeC = store::StorageControler::Intance();
    }
    
LightControler::~LightControler()
    {
    }

void LightControler::addLight(String light,int* channelList,lightModel::lightType type){
    _logger->logDebug(module,"Add Light "+light);
    lightList[light]= new lightModel::LightModel(type,channelList);
    _logger->logDebug(module,"Light added succesful"+light);
}

void LightControler::removeLight(String light){
    lightModel::LightModel* model =lightList[light];
    lightList.erase(light);
    delete model;
}

void LightControler::changeLight(String light,int* channelList,lightModel::lightType type){
    removeLight(light);
    addLight(light,channelList,type);
}

void LightControler::setLight(String light,hsb _hsb,int status){
    _logger->logDebug(module,"set Light "+light+" to Status "+String(status)+" with HSB: "+String(_hsb.h)+String(_hsb.s)+String(_hsb.b));
    lightModel::LightModel* lM = lightList[light];
    int* valueSet;
    int vLenght;
    if(lM->getType() == lightModel::lightType::rgbw){
        _logger->logDebug(module,"IF RGBW ");
        vLenght=4;
        int rgbw[4];
        valueSet = rgbw;
        hsbToRGBW(_hsb,rgbw);
    }
    if(lM->getType() == lightModel::lightType::rgb){
        _logger->logDebug(module,"IF RGB ");
        vLenght=3;
        int rgb[3];
        valueSet = rgb;
        hsbToRGB(_hsb,rgb);
    }
    lM->set(status,valueSet);
     _logger->logDebug(module,"Light Setted ");
    for( int i = 0; i< vLenght; i++ ){
        if(status){
            pwmControler->setChannel(*(lM->getChannels()+i),*(lM->getValue()+i));
        }else{
            pwmControler->setChannel(*(lM->getChannels()+i),0);
        }
    }
    _logger->logDebug(module,"PWM Setted ");
}

void LightControler::setLight(String light,int _status){
     setLight(light,getHsb(light),_status);
}

hsb LightControler::getHsb(String light){
    float r = (lightList[light]->getValue()[0])/4095.0;
    float g = (lightList[light]->getValue()[1])/4095.0;
    float b = (lightList[light]->getValue()[2])/4095.0;
    float cMax = max(r,max(g,b));
    float cMin = min(r,min(g,b));
    float delta = cMax -cMin;
    float h =0.0;
    if(cMax==r) h = fmod(360+60*fmodf(((g-b)/delta),6),360);
    if(cMax==g) h = 60*(((b-r)/delta)+2);
    if(cMax==b) h = 60*(((r-g)/delta)+4);
    if(delta==0.0) h =0.0;
    float B = cMax;
    float s = 0.0;
    if(cMax>0) s = delta/cMax; 
    hsb _hsb;
    _hsb.b = (B*100.0);
    _hsb.h = h;
    _hsb.s = (s*100.0);
    _logger->logDebug(module,"rgbTo hsb: "+String(_hsb.h)+", "+String(_hsb.s)+", "+String(_hsb.b));
    return _hsb;

}


hsb LightControler::getHsbOld(String light){
    float r = (lightList[light]->getLastvalue()[0])/4095.0;
    float g = (lightList[light]->getLastvalue()[1])/4095.0;
    float b = (lightList[light]->getLastvalue()[2])/4095.0;
    float cMax = max(r,max(g,b));
    float cMin = min(r,min(g,b));
    float delta = cMax -cMin;
    float h =0.0;
    if(cMax==r) h = fmod(360+60*fmodf(((g-b)/delta),6),360);
    if(cMax==g) h = 60*(((b-r)/delta)+2);
    if(cMax==b) h = 60*(((r-g)/delta)+4);
    if(delta==0.0) h =0.0;
    float s = 0.0;
    if(cMax>0) s = delta/cMax; 
    hsb _hsb;
    _hsb.b = 0.0;
    _hsb.h = roundf(h);
    _hsb.s = roundf((s*100.0));
    _logger->logDebug(module,"rgbTo hsb: "+String(_hsb.h)+", "+String(_hsb.s)+", "+String(_hsb.b));
    return _hsb;

}
int LightControler::getStatus(String light){
    return lightList[light]->getStatus();
}

void LightControler::setPwmControler(pwm::PwmController* _pwmControler){
    pwmControler= _pwmControler;
}
void LightControler::hsbToRGB(hsb _hsb, int* valueList){
    float r, g, b;
    r=0.0;
    g=0.0;
    b=0.0;
    float c = (_hsb.s * _hsb.b)/10000.0;
    float x = (c*(1-fabs(fmod((_hsb.h/60),2)-1)));
    if(_hsb.h>=0 && _hsb.h<60){
        r=c;
        g=x;
        b=0;
    }
    if(_hsb.h>=60 && _hsb.h<120){
        r=x;
        g=c;
        b=0;
    }
    if(_hsb.h>=120 && _hsb.h<180){
        r=0;
        g=c;
        b=x;
    }
    if(_hsb.h>=180 && _hsb.h<240){
        r=0;
        g=x;
        b=c;
    }
    if(_hsb.h>=240 && _hsb.h<300){
        r=x;
        g=0;
        b=c;
    }
    if(_hsb.h>=300 && _hsb.h<=360){
        r=c;
        g=0;
        b=x;
    }
    float m = (_hsb.b/100.0)-c;
    r =(r+m)*4095;
    g=(g+m)*4095;
    b=(b+m)*4095;
  valueList[0]=(int)r;
  valueList[1]=(int)g;
  valueList[2]=(int)b;
  _logger->logDebug(module,"HSB Just RGB ");
  _logger->logDebug(module,"HSB end: CH0: "+String(valueList[0])+" CH 1: "+String(valueList[1])+" CH 2: "
                                                +String(valueList[2]));


}
void LightControler::hsbToRGBW(hsb _hsb, int* valueList){
    float r, g;
    r=0.0;
    g=0.0;
    float s=1.0;
    float b=1.0;
    if((_hsb.s/100.0)>0.5) s = 1;
    else s =_hsb.s/50;
    float c = (s * _hsb.b)/100.0;
    float x = (c*(1-fabs(fmod((_hsb.h/60),2)-1)));
    if(_hsb.h>=0 && _hsb.h<60){
        r=c;
        g=x;
        b=0;
    }
    if(_hsb.h>=60 && _hsb.h<120){
        r=x;
        g=c;
        b=0;
    }
    if(_hsb.h>=120 && _hsb.h<180){
        r=0;
        g=c;
        b=x;
    }
    if(_hsb.h>=180 && _hsb.h<240){
        r=0;
        g=x;
        b=c;
    }
    if(_hsb.h>=240 && _hsb.h<300){
        r=x;
        g=0;
        b=c;
    }
    if(_hsb.h>=300 && _hsb.h<=360){
        r=c;
        g=0;
        b=x;
    }
    float m = (_hsb.b/100.0)-c;
    r =(r+m)*4095;
    g=(g+m)*4095;
    b=(b+m)*4095;
    float w = 0.0;
    // if(_hsb.s<50)   
    w = fmin((1.0-(_hsb.s/100))*2,1)*(_hsb.b/100.0)*4095.0;
    // if(_hsb.s==0.0){
    //     r=g=b=w=4095;
    // }
    _logger->logDebug(module,"HSBW post ");
    valueList[0]=(int)r;
    valueList[1]=(int)g;
    valueList[2]=(int)b;
    valueList[3]=(int)w;
    _logger->logDebug(module,"HSBW end: CH0: "+String(valueList[0])+" CH 1: "+String(valueList[1])+" CH 2: "
                                                +String(valueList[2])+" CH 3: "+String(valueList[3]));
}

lightModel::LightModel* LightControler::getLight(String lightName){
     return  lightList[lightName];
}
//TODO move to light controler
void LightControler::storeLight(String lightName,String _file){
    _logger->logDebug(module,"Store Lights");
    DynamicJsonDocument doc(2000);
    storeC->loadJson(&doc,_file);
    JsonArray lights;
    if(doc.containsKey("lights")){
         _logger->logDebug(module,"Lights key found");
        lights = doc["lights"];
    }else{
         _logger->logDebug(module,"Create light array");
        lights =doc.createNestedArray("lights");
    }
    int count=-1;
    for(auto light:lights){
        count++;
        if(light["name"].as<String>().compareTo(lightName)==0){
            break;
        }
        
    }
    _logger->logDebug(module,"found index "+count);
    DynamicJsonDocument light(200);
    light["name"]=lightName;
    lightModel::LightModel* lightModel = getLight(lightName);
    JsonArray channels = light.createNestedArray("channels");
    _logger->logDebug(module,"loopStart ");
    int* ch =lightModel->getChannels();
    for(int index=0;index <3; index++){
        _logger->logDebug(module,"lightindes "+String(index)+" "+String(ch[index]));
        channels.add(ch[index]);
    }
    _logger->logDebug(module,"loopEnd ");
    if(lightModel->getType()==lightModel::lightType::rgbw){
        light["type"]="rgbw";
        channels.add(ch[3]);
    }else{
        light["type"]="rgb";
    }
    _logger->logDebug(module,"IFEnd ");
    char JSONmessageBuffer[400];
    serializeJson(light, JSONmessageBuffer);
    _logger->logDebug(module,"light name "+String(JSONmessageBuffer));
    if(!lights[count].isNull()){
        lights[count].set(light);
    }else lights.add(light);

    storeC->storeJson(&doc,_file);
    
}
void LightControler::loadLights(String _file){
    _logger->logDebug(module,"Load lights");
    DynamicJsonDocument doc(2000);
    storeC->loadJson(&doc,_file);
    if(doc.containsKey("lights")){
        JsonArray lights = doc["lights"];
        for(auto light:lights){
            String lightName = light["name"].as<String>();
            JsonArray channels = light["channels"].as<JsonArray>();
            int _channel[channels.size()];
            _logger->logDebug(module,"Json array size "+String(channels.size())+" !");
            for(u_int index=0;index<channels.size();index++){
                _channel[index]=channels[index].as<int>();
            }
            lightModel::lightType lightType;
            String type= light["type"];
            if(type.compareTo("rgbw")==0){
                lightType= lightModel::lightType::rgbw;
            }else{
                lightType= lightModel::lightType::rgb;
            }
            addLight(lightName,_channel,lightType);
        }
        #ifdef debug
            for(auto light:lightList){
                _logger->logDebug(module,"Light from Json "+light.first+" , type "+light.second->getType()+
                " , channels "
                +String(light.second->getChannels()[0])+", "
                +String(light.second->getChannels()[1])+", "
                +String(light.second->getChannels()[2])+", "
                +String(light.second->getChannels()[3]));
            }
        #endif
    }
}

std::map<String,lightModel::LightModel*>* LightControler::getLights(){
    return &lightList;
}