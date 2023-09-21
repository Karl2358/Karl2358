#include "MqttControler.h"
#include "LightControler.h"
#include <TaskSchedulerDeclarations.h>
#include <functional>
#include <ArduinoJson.h>
#include <StatusLed.h>

using namespace mqtt;


 MqttControler::MqttControler(String _server, int _port,String _user, String _pw,Scheduler* sched):
                                server(_server), port(_port),user(_user),pw(_pw)
    {
        _logger = logger::Logger::Intance();
        _logger->logInfo(modul,"INIT");
        mqttCTask.set(10*TASK_MILLISECOND,TASK_FOREVER,[this](){
            // _logger->logDebug(modul,"runTask");
            run();
        });
        sched->addTask(mqttCTask);
        mqttCTask.enable();
        client = new PubSubClient(espClient);
        _logger->logDebug(modul,"Connet to "+String(server)+", Port "+port);
        client->setServer(server.c_str(),port );
        client->setCallback([this](char* c_toppic, byte* payload, unsigned int length){
                callback(c_toppic, payload, length);});
        _logger->logDebug(modul,"connected ? "+String(client->connected()));
        
        
    }
    
MqttControler::~MqttControler()
    {
    }

void MqttControler::addMqttClient(String ligthName){
    _logger->logDebug(modul,"Add client "+ligthName);
    clientList[ligthName]=new MqttClient(ligthName,this);
    // clientList[ligthName]=new mqttClient::MqttClient(ligthName);
}


void MqttControler::removeClient(String ligthName){
    delete (clientList[ligthName]);
    clientList.erase(ligthName);
}

void MqttControler::run(){
    if(!client->connected()){
        reconnect();
    }
     client->loop();
}
void MqttControler::callback(char* topic, byte* payload, unsigned int length){ 
    String msg = String((char*)payload).substring(0,length);
    String _topic = String(topic);
    _topic=_topic.substring(0,_topic.indexOf('/'));
    _logger->logDebug(modul,"got callback for topic "+String(topic)+", parsed topic="+_topic);
    auto light = clientList.find(_topic);
    if(light!=clientList.end()){
        light->second->callback(&_topic,&msg,length,client);
    }else{
        _logger->logInfo(modul,"Mqtt Client "+_topic+" not found");
    }

}
void MqttControler::setLight(String linghtName,lightControler::hsb hsb, int status){
    
    lightCtrl->setLight(linghtName,hsb,status);
}

String MqttControler::getLight(String lightName){
    int status =lightCtrl->getStatus(lightName);
    lightControler::hsb hsb = lightCtrl->getHsb(lightName);
    DynamicJsonDocument doc(200);
    JsonArray jhsb = doc.createNestedArray("hsb");
    if(status){
        doc["state"]="ON";
    }else{
        doc["state"]="OFF";
        hsb = lightCtrl->getHsbOld(lightName);
    }
    
    jhsb.add(hsb.h);
    jhsb.add(hsb.s);
    jhsb.add(hsb.b);
    char JSONmessageBuffer[400];
    serializeJson(doc, JSONmessageBuffer);
    doc.clear();
    return String(JSONmessageBuffer);
}

void MqttControler::reconnect() {
  // Loop until we're reconnected
  status::StatusLed::getSatusLed()->set(1,3);

  if (!client->connected()) {
     _logger->logInfo(modul,"Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connectOuter
    _logger->logDebug(modul,"connect with "+getUser()+", "+getPW());
    // _logger->logDebug(modul,"ping: "+String( Ping.ping(ctrl->getServer().c_str())));
    if (client->connect(clientId.c_str(),user.c_str(),pw.c_str())) {
       _logger->logInfo(modul,"connected");
      // Once connected, publish an announcement...
      client->publish(String("toppic/get").c_str(), "hello world");
      // ... and resubscribe
      subscribe(client);
      mqttCTask.setInterval(10*TASK_MILLISECOND);
      status::StatusLed::getSatusLed()->set(1,10);
      reconnectCounter = 0;
    } else {
       _logger->logInfo(modul,"failed, rc=");
       _logger->logInfo(modul,String(client->state()));
       _logger->logInfo(modul," try again in 5 seconds");
       reconnectCounter++;
      // Wait 5 seconds before retrying
      mqttCTask.setInterval(5*TASK_SECOND);
    }
    if(reconnectCounter >10){
        ESP.restart();
    }
  }
}
void MqttControler::subscribe(PubSubClient* _client){
    for(auto& it:clientList){
        // _logger->logDebug(modul,"itterate ");
        _client->subscribe((it.first+"/set").c_str());
        _logger->logDebug(modul,"Subcribe to "+(it.first+"/set"));
    }
}

void MqttControler::setLightCtrl(lightControler::LightControler* _lightCtrl){
     lightCtrl=_lightCtrl;
}
void MqttControler::setLightCtrl(lightControler::LightControler* _lightCtrl, boolean _subscribeAll){
    setLightCtrl(_lightCtrl);
    if(_subscribeAll){
        for(auto light:*(_lightCtrl->getLights())){
            addMqttClient(light.first);
        }
    }
}

void MqttControler::publishUpdate(){
    for(auto elem : clientList){
        elem.second->publishUpdate(elem.first,client);
    }
}