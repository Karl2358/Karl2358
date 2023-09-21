#ifndef STORAGE_CONTROLER_H
#define STORAGE_CONTROLER_H
#include "Logger.h"
#include <Arduino.h>
#include <ArduinoJson.h>
namespace store{
    class StorageControler
    {
    private:
        /* data */
        String module="Storage Controler";
        logger::Logger* _logger;
        StorageControler(/* args */);
        ~StorageControler();
        static StorageControler* m_pInstance;
    public:
        
        static StorageControler* Intance();
        void loadJson(JsonDocument* doc,String file);
        void storeJson(JsonDocument* doc, String file);
        void storeLight(String lightName, String file);
    };
    
   
    
}
#endif 