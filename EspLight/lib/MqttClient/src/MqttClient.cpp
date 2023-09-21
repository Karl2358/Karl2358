#include "MqttClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <ArduinoJson.h>
#include "LightControler.h"
#include "StorageControler.h"

using namespace mqtt;

MqttClient::MqttClient(String _toppic,mqtt::MqttControler* _ctrl):
  // MqttClient::MqttClient(String _toppic):
  toppic(_toppic),ctrl(_ctrl)
    {
        _logger = logger::Logger::Intance();
        _logger->logInfo(modul,"Create for toppic "+toppic);
        _logger->logInfo(modul,"finish init");
}
    
MqttClient::~MqttClient()
    {
    }

void MqttClient::callback(String* topic, String* msg, unsigned int length, PubSubClient* client) {
  _logger->logInfo(modul,"Message for "+(*topic)+" arrived:\n"+*msg);
  DynamicJsonDocument doc(200);
  DeserializationError error = deserializeJson(doc,*msg);
  if(error){
    _logger->logInfo(modul+" "+*topic,"Deserialization Error on message:\n"+*msg);
    return;
  }
  int state=0;
  if(strcmp(doc["state"],"ON")==0)state=1;
  lightControler::hsb hsb;
  hsb.h=doc["hsb"][0];
  hsb.s=doc["hsb"][1];
  hsb.b=doc["hsb"][2];
  if(hsb.b==0)state=0;
  ctrl->setLight(*topic,hsb,state);
  client->publish(String((*topic)+"/get").c_str(),(ctrl->getLight((*topic))).c_str());
  // _logger->logInfo(modul+" "+toppic,"State: "+String(doc["state"].as<char*>()));
  // _logger->logInfo(modul+" "+toppic,doc["hsb"]);
  // _logger->logInfo(modul+" "+toppic,"h: "+String(hsb.h));
  // _logger->logInfo(modul+" "+toppic,"s: "+String(hsb.s));
  // _logger->logInfo(modul+" "+toppic,"b: "+String(hsb.b));
  // _logger->logInfo(modul+" "+toppic,"ENDE");
  doc.clear();
  }

void MqttClient::publishUpdate(const String topic,PubSubClient* client){
  client->publish(String(topic+"/get").c_str(),(ctrl->getLight(topic)).c_str());
}
