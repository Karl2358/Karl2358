#include "StorageControler.h"
#include <LittleFS.h>
//#define debug
using namespace store;



StorageControler* StorageControler::m_pInstance=NULL;

StorageControler* StorageControler::Intance(){
    if(!m_pInstance){
        m_pInstance = new StorageControler();
    }
    return m_pInstance;
}

StorageControler::StorageControler(/* args */)    {
        _logger = logger::Logger::Intance();
        if (!LittleFS.begin()){
                // Serious problem
            _logger->logDebug(module,"LittleFS Mount failed");
        }else{
            _logger->logDebug(module,"LittleFS Mount sucessful");
        }
}
    
StorageControler::~StorageControler()    {
        LittleFS.end();
}
void StorageControler::loadJson(JsonDocument* doc, String fileName){
    _logger->logDebug(module,"Load JSon");
    File file = LittleFS.open(fileName.c_str(),"r");
    _logger->logDebug(module,"Loaded File" +fileName)
    ;
    if(!file){
        _logger->logInfo(module,"File "+fileName+" not found");
    }else{
        int size = file.size();
        if(size==0){
            _logger->logInfo(module,"File "+fileName+" is empty");
        }else{
            deserializeJson(*doc,file.readString());
        }
        file.close();
    }
    _logger->logDebug(module,"Load complete");
}
void StorageControler::storeJson(JsonDocument* doc, String fileName){
    _logger->logDebug(module,"Store Json");
    File file = LittleFS.open(fileName.c_str(),"w");
    if(!file){
        _logger->logInfo(module,"File "+fileName+" opened");
    }else{
        serializeJson(*doc,file);
        file.close();
        #ifdef debug
            file = LittleFS.open(fileName.c_str(),"r");
            _logger->logDebug(module,file.readString());
        #endif
    }
}
