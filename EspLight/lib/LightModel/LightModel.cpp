#include "LightModel.h"
using namespace lightModel;

LightModel::LightModel(lightType type, int* _channelList):type(type)
    {
        _logger=logger::Logger::Intance();
        // _logger->logDebug(module," initialized CH:"+String(_channelList[0])+String(_channelList[1])+String(_channelList[2])+String(_channelList[3]));
        if(type==lightModel::lightType::rgb){
            // _logger->logDebug(module,"RGB");
            channelList = new int[3];
            memcpy(channelList,_channelList, 12);
            valueList = new int[3];
            lastValueList= new int[3];
        }else{
            // _logger->logDebug(module,"RGBW");
            channelList = new int[4];
            memcpy(channelList, _channelList, 16);
            valueList= new int[4];
            lastValueList= new int[4];
        }
    }
    
LightModel::~LightModel()
    {
    }

void LightModel::set(int _status){
    status=_status;
};
void LightModel::set(int _status, int* _valueList){
    // _logger->logDebug(module,"ligth set: status "+String(_status)+" Valueset: "+String(_valueList[0])+","
    //                     +String(_valueList[1])+","+String(_valueList[2])+","+String(_valueList[3]));
    status=_status;
    int length =16;
    if(type==lightModel::rgb){
        length = 12;
    }
    memcpy(lastValueList,valueList,length);
    memcpy(valueList,_valueList,length);
    // _logger->logDebug(module,"setted: status "+String(status)+" Valueset: "+String(valueList[0])+","
    //                     +String(valueList[1])+","+String(valueList[2])+","+String(valueList[3]));
    
}