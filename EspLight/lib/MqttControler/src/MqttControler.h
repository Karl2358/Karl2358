#ifndef MQTT_CONTROLER_H
#define MQTT_CONTROLER_H
#include <PubSubClient.h>
#include "Logger.h"
#include "MqttControler.fwd.h"
#include "MqttClient.h"
#include "LightControler.h"
#include "StorageControler.h"
#include <TaskSchedulerDeclarations.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>




//#include "LightController.h"


#include <map>

namespace mqtt{
    //class  MqttClient;
    class MqttControler
    {
    private:
        /* data */
        logger::Logger* _logger;
        String server;
        int port;
        String user;
        String pw;
        String modul = "Mqtt Controler";  
        std::map<String,MqttClient*> clientList;
        Task mqttCTask;
        void reconnect();
        store::StorageControler* _storeC;
        PubSubClient* client;
        WiFiClient espClient;
        void callback(char* topic, byte* payload, unsigned int length);
        void subscribe(PubSubClient* _client);
        int reconnectCounter = 0;
        //lightControler::LightControler* lightCtrl;

    public:
    lightControler::LightControler* lightCtrl;
        MqttControler(String _server, int _port,String _user, String _pw,Scheduler* sched);
        ~MqttControler();
        String getServer(){return server;};
        String getUser(){return user;};
        String getPW(){return pw;};
        int getPort(){return port;};
        void addMqttClient(String lightName);
        void removeClient(String ligthName);
        void run();
        void setLight(String linghtName,lightControler::hsb hsb, int status);
        String getLight(String lightName);
        void publishUpdate();
        void setLightCtrl(lightControler::LightControler* _lightCtrl);
        void setLightCtrl(lightControler::LightControler* _lightCtrl,boolean _subcribeall);
    };
    
   
    
}
#endif