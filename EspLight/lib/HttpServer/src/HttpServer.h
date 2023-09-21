
#ifndef HTTPSERVER_H // include guard
#define HTTPSERVER_H
#include <ESP8266WebServer.h>
#include <map>
#include <ArduinoJson.h>
#include <Logger.h>
#include <Arduino.h>

class HttpServer{
public:
  void setup(int port);
  void run();
private:
  String modul = "HttpServer";
  logger::Logger* _logger;
  void handleRoot();
  void handleUnknown();
  void notFound();
  ESP8266WebServer* server;

};

#endif
