#include "ota.h"
#include <ArduinoOTA.h>
#include <LittleFS.h>
using namespace ota;

otaRunner::otaRunner(/* args */)
{
    ArduinoOTA.onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH) {
                        type = "sketch";
                } else { // U_LittleFS
                        LittleFS.end();
                        type = "filesystem";
                }

                // NOTE: if updating LittleFS this would be the place to unmount LittleFS using LittleFS.end()
#ifdef  debug
                Serial.println("Start updating " + type);
  #endif
        });
        ArduinoOTA.onEnd([]() {
#ifdef  debug
                Serial.println("\nEnd");
  #endif
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
#ifdef  debug
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  #endif
        });
        ArduinoOTA.onError([](ota_error_t error) {
#ifdef  debug
                Serial.printf("Error[%u]: ", error);
  #endif
                if (error == OTA_AUTH_ERROR) {
#ifdef  debug
                        Serial.println("Auth Failed");
  #endif
                } else if (error == OTA_BEGIN_ERROR) {
#ifdef  debug
                        Serial.println("Begin Failed");
  #endif
                } else if (error == OTA_CONNECT_ERROR) {
#ifdef  debug
                        Serial.println("Connect Failed");
  #endif
                } else if (error == OTA_RECEIVE_ERROR) {
#ifdef  debug
                        Serial.println("Receive Failed");
  #endif
                } else if (error == OTA_END_ERROR) {
#ifdef  debug
                        Serial.println("End Failed");
  #endif
                }
        });
        ArduinoOTA.begin();
#ifdef  debug
        Serial.println("Ready for OTA UPDATE2");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
  #endif
}

otaRunner::~otaRunner()
{
}

void otaRunner::run(){
    ArduinoOTA.handle();
}