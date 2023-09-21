#include "WifiService.h"
#include "StatusLed.h"
using namespace wifi;

WifiService* WifiService::m_pInstance=NULL;


WifiService* WifiService::Intance(){
    if(!m_pInstance){
        m_pInstance = new WifiService();
    }
    return m_pInstance;
}

void WifiService::setPsk(String _psk){
    psk = _psk;
}

void WifiService::setBssid(String _bssid){
    bssid = _bssid;
}

void WifiService::setMode(WiFiMode_t _mode){
    mode=_mode;
}

WifiService::WifiService(){
    _logger = logger::Logger::Intance();
    WiFi.mode(mode);
    if(psk!=""&& bssid!=""){
        start();
    }
}

void WifiService::start(){
    WiFi.begin(bssid,psk);
    int counter = 0;
    while(WiFi.status() != WL_CONNECTED ){
        status::StatusLed::getSatusLed()->set(1,1);
        _logger->logInfo(modul,"Connecting..");
        delay ( 500 );
        if(counter++ >10){
            SPIFFS.end();
            ESP.restart();
        }
    }
    status::StatusLed::getSatusLed()->set(1,10);
    String ip = WiFi.localIP().toString();
    _logger->logInfo(modul,String("Connected with ip ")+ip);

}

void WifiService::stop(){
    
}