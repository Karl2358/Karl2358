#ifndef MQTT_CLIENT
#define MQTT_CLIENT
#include "Logger.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "MqttClient.fwd.h"
#include "MqttControler.h"
#include <TaskSchedulerDeclarations.h>

namespace mqtt{
    class MqttClient
    {
    private:
        /* data */
        logger::Logger* _logger; 
        String toppic = "base";
        String modul = "Mqtt client";  
        MqttControler* ctrl;
        //void reconnect();
        // WiFiClient espClient;
    public:
        MqttClient(String _toppic, mqtt::MqttControler* ctrl);
        // MqttClient(String _toppic);
        ~MqttClient();
        void publishUpdate(const String topic,PubSubClient* client);
        void callback(String* topic, String* payload, unsigned int length,PubSubClient* client);
        // void run();
    };
    
    
    
}
#endif