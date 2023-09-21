
#include "HttpServer.h"
#include <ESP8266WebServer.h>
#include <FS.h>
#define debug

// Création des objets / create Objects


void HttpServer::setup(int port) {
  server = new  ESP8266WebServer(port);
  // Connexion WiFi établie / WiFi connexion is OK
  _logger = logger::Logger::Intance();
  _logger->logInfo(modul,"Init");
  _logger->logInfo(modul,"IP addressHTTP: "+WiFi.localIP().toString() ); 

  // if (!SPIFFS.begin())
  // {
  //   // Serious problem
  //   Serial.println("SPIFFS Mount failed");
  // } else {
  //   Serial.println("SPIFFS Mount succesfull");
  // }
  server->onNotFound([this](){handleUnknown();});
  // server->serveStatic("/main-es2015.js", SPIFFS, "/main-es2015.js.gz","contet-type:application/javascript");
  // server->serveStatic("/main-es5.js", SPIFFS, "/main-es5.js.gz");
  server->on("/", [this](){handleRoot();}); 
  // server->on("/delay",[this](){
  //     server->send(200, "text/html", "Delay triggert");
  //     _logger->logInfo(modul, "triger Delay");
  //     while(true) {}
  //     ;});
  // server->serveStatic("/favicon.ico", SPIFFS, "/favicon.ico.gz");


  server->begin();
  _logger->logInfo(modul,"Server Started");

}
void HttpServer::handleRoot() {
  Dir dir = SPIFFS.openDir("/logs");
  String files = "";
  while(dir.next()){
    files = files+"<a href=\""+dir.fileName().substring(1)+"\">"+dir.fileName()+"</a><br>";
  }
  server->send(200, "text/html", "Hello world! <br>"+files);

}

// Sendet "Not Found"-Seite
void HttpServer::notFound(){ 
  String HTML = F("<html><head><title>404 Not Found</title></head><body>"
                  "<h1>Not Found</h1>"
                  "<p>The requested URL was not found on this webserver.</p>"
                  "</body></html>");
  server->send(404, "text/html", HTML);
}

// Es wird versucht, die angegebene Datei aus dem SPIFFS hochzuladen
void HttpServer::handleUnknown(){
   String filename = server->uri();

  File pageFile = SPIFFS.open(filename, "r");
  if (pageFile){
    String contentTyp = "";
    if(filename.endsWith(".ico")){
      contentTyp= "image/x-icon";
      #ifdef debug
      _logger->logDebug(modul,"Requeseted icon: "+filename);
      #endif
    }
    if(filename.endsWith(".css")){
      contentTyp= "text/css";
      #ifdef debug
     _logger->logDebug(modul,"Requeseted css: "+filename);
      #endif
    }
    if(filename.endsWith(".js")){
      contentTyp= "application/javascript";
      #ifdef debug
      _logger->logDebug(modul,"Requeseted js: "+filename);
      #endif
    }
    server->streamFile(pageFile, contentTyp);
    pageFile.close();
  }
  else
    notFound();
}


void HttpServer::run(){
  server->handleClient();
  }

