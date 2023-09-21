#include <Arduino.h>
#include <Logger.h>
#include <WifiService.h>
#include <PwmControler.h>
#include <LightControler.h>
#include <LightModel.h>
#include <MqttControler.h>
#include <TaskScheduler.h>
#include <functional>
#include <ota.h>
#include <StorageControler.h>
#include <StatusLed.h>
#include <HttpServer.h>
#include <FS.h>



logger::Logger* _logger;
wifi::WifiService* _wifi;
pwm::PwmController* _pwm;
ota::otaRunner otaR;
Scheduler sched;
lightControler::LightControler* _lightC;
mqtt::MqttControler* mqttCtrl;
store::StorageControler* _storageC;
HttpServer _HttpServer;

//real
String module = String("Main Module");
Task test(10*TASK_SECOND,100,[](){
  Serial.println("test2");
},&sched);

Task update(10*TASK_SECOND,TASK_FOREVER,[](){
  mqttCtrl->publishUpdate();
},&sched);

Task reset(TASK_IMMEDIATE,1,[](){
  _logger->logDebug("Daily reset","Resetting...");
  SPIFFS.end();
  ESP.restart();
},&sched);

Task getHeap(60*TASK_SECOND,TASK_FOREVER,[](){
  int freeHeap = ESP.getFreeHeap();
  if(freeHeap<1300){
    _logger->logDebug("HeapChecker","free Heapsize: "+String(freeHeap)+"KB");
    _logger->logDebug("HeapChecker","RESTART.....");
    SPIFFS.end();
    ESP.restart();
  }
},&sched);

Task checkWifi(10*TASK_SECOND,TASK_FOREVER,[](){
  // _logger->logDebug("check Wifi",String(WiFi.isConnected()));
  if(!WiFi.isConnected()){
    _logger->logDebug("check Wifi",String(WiFi.isConnected()));
    _logger->logDebug("check Wifi", "Restart...");
    SPIFFS.end();/*  */
    ESP.restart();
  }
},&sched);

void taskResetSerial(){
        Serial.end();

        Serial.begin(250000);
        Serial.println("reset Serial");
}

Task resetSerial(10*TASK_MINUTE,TASK_FOREVER,[](){
        _logger->logDebug("reset Serial","Resetting...");
        Serial.end();
        Serial.begin(250000);
        _logger->logDebug("reset Serial","reset Serial");
},&sched);

Task rHTTP(100*TASK_MILLISECOND,TASK_FOREVER,[](){
        _HttpServer.run();
},&sched);


// void blink();
// Task blinking(10*TASK_SECOND,TASK_FOREVER,blink,&sched);
// bool toggle=true;

// void blink(){
//   Serial.print("blink ");
//   Serial.println(toggle);
//   if(toggle){
//     digitalWrite(2,HIGH);
//     blinking.setInterval(1*TASK_SECOND);
//     toggle = !toggle;
//   }else{
//     digitalWrite(2,LOW);
//     blinking.setInterval(10*TASK_SECOND);
//     toggle = !toggle;

//   }
// }


void setup() {
  ESP.wdtEnable(1000);
  status::StatusLed* _status = status::StatusLed::getSatusLed(&sched);
  _logger = logger::Logger::Intance();
  _wifi = wifi::WifiService::Intance();
  _storageC = store::StorageControler::Intance();
  DynamicJsonDocument doc(1000);
  _storageC->loadJson(&doc,"/config.json");
  _wifi->setBssid(doc["BSSID"]);
  _wifi->setPsk(doc["PW"]);
  _wifi->start();
  _pwm = new pwm::PwmController();

  
  _status->set(1,10);
  _lightC = new lightControler::LightControler(_pwm); //
  mqttCtrl = new mqtt::MqttControler(doc["MQTT_SERVER"].as<String>(),doc["MQTT_PORT"].as<int>(),doc["MQTT_USER"].as<String>(),doc["MQTT_PW"].as<String>(),&sched);
  // int lightChannel[4]={0,1,9,3};
  // int lightChannel2[4]={4,5,6,7};
  // _lightC->addLight("testLight2",lightChannel,lightModel::lightType::rgbw);
  // _lightC->addLight("DinningInner2",lightChannel2,lightModel::lightType::rgbw);
  // mqttCtrl->addMqttClient("testLight2");
  // // int lightChannel2[4]={4,5,6,7};
  // // _lightC->addLight("testLight3",lightChannel2,lightModel::lightType::rgbw);
  // mqttCtrl->addMqttClient("DinningInner2");
  // _lightC->storeLight("testLight2","/config.json");
  _lightC->loadLights("/config.json");
  mqttCtrl->setLightCtrl(_lightC,true);
  _HttpServer.setup(8080);
  _logger->logDebug(module,String("Setup Ende"));
  // test.enableIfNot();
  // reset.enableDelayed(7*24*60*TASK_MINUTE);
  resetSerial.enableIfNot();
  getHeap.enableIfNot();
  checkWifi.enableIfNot();
  rHTTP.enableIfNot();
  update.enableDelayed(10*TASK_SECOND);
}
lightControler::hsb _hsb;
void loop() {
  // put your main code here, to run repeatedly:
  otaR.run();
  sched.execute(); 
  ESP.wdtFeed();
  // Serial.print("FreeHeap");
  // Serial.println(ESP.getFreeHeap());
  // Serial.print("Heap Fragmentaion");
  // Serial.println(ESP.getHeapFragmentation());
  // Serial.print("free block");
  // Serial.println(ESP.getMaxFreeBlockSize());
  // ESP.wdtFeed();

  // mqttCtrl->run();
  // mqttCtrl->run();  
  // _hsb.h = 359;
  // _hsb.s = 100;
  // _hsb.b = 100;
  // _lightC->setLight("test",_hsb,1);
  // delay(5000);
  // _hsb.b = 10;
  // _lightC->setLight("test",_hsb,1);
  // delay(5000);

  
}