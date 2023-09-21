#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Logger.h"

namespace wifi{
    class WifiService{
        public:
            static WifiService* Intance();  
            void setBssid(String ssidName );
            void setPsk(String psk);
            void setMode(WiFiMode_t mode);
            void start(void);
            void stop(void);

        private:
            WifiService();
            String modul="Wifi Modul";
            static WifiService* m_pInstance;
            String psk = "";
            String bssid = "";
            WiFiMode_t mode = WIFI_STA;
            logger::Logger* _logger;

    };
}
#endif