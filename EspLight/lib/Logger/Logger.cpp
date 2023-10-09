#include "Logger.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <math.h>
using namespace logger;


Logger* Logger::m_pInstance = NULL;

Logger* Logger::Intance(){
    if(!m_pInstance){
        m_pInstance = new Logger();
    }
    return m_pInstance;
}

Logger::Logger(){
    Serial.begin(250000);
    if( Serial.isTxEnabled()) {
    Serial.print("Logger instanceiated, with logging level: ");

        }
        else{
            logToFile("Logger", "Serial not Conecced, log only to File");
        }
    if(loggingLevel==Debug){
    if( Serial.isTxEnabled()) {
        Serial.println("debug");

        }
    }
    if(loggingLevel==Info){
    if( Serial.isTxEnabled()) {
         Serial.println("info");

        }
    }
    // String logEntry = "log";
    //     // logEntry.replace(" ","");
    // f = LittleFS.open("/logs/"+logEntry+".txt", "a");
    
    
}

void Logger::logDebug(String module, String msg){
    

    if(loggingLevel==Debug){
        if(loggingToFile==1) logToFile(module, msg);
        String logEntry = String(module);
        module += " DEBUG :    ";
    if( Serial.isTxEnabled()) {
        Serial.print(module.c_str());
        Serial.println(msg.c_str());

        }    }
    
}
void Logger::logInfo(String module, String msg){
    

    if(loggingLevel==Debug || loggingLevel==Info){
        if(loggingToFile==1) logToFile(module, msg);
        module += " INFO :    ";
    if( Serial.isTxEnabled()) {
        Serial.print(module.c_str());
        Serial.println(msg.c_str());

        }
    }
    
}

void Logger::logToFile(String module, String msg){
        // FSInfo fs_info;
        // LittleFS.info(fs_info);
        // Serial.print("Free Space on FS: ");
        // Serial.println(fs_info.totalBytes-fs_info.usedBytes);
        //String logEntry = String(module);
        String logEntry = "log";
        // // logEntry.replace(" ","");
        f = LittleFS.open("/logs/"+logEntry+".txt", "a");
        if(f.size()>100000){
            // Serial.print("File Size FS: ");
            // Serial.println(f.size());
            f.close();
            LittleFS.remove("/logs/"+logEntry+"_OLD.txt");
            LittleFS.rename("/logs/"+logEntry+".txt","/logs/"+logEntry+"_OLD.txt");
            f = LittleFS.open("/logs/"+logEntry+".txt", "w");
        }
        if(loggingLevel==0){
            module += " DEBUG";
        }else{module+=" INFO";}
        module += ":    ";
        f.print(timeStamp());
        f.print(" ");
        f.print(module.c_str());
        f.println(msg.c_str());
        f.close();
}

String Logger::timeStamp(void){
    String out = "";
    int mil = millis();
    int h = (mil/3600000);
    int m = (mil-h*3600000)/60000;
    int s = (mil-(h*3600000 + m*60000))/1000;
    int mi= (mil-(h*3600000 + m*60000+s*1000));
    out += String(h)+":"+String(m)+":"+String(s)+":"+String(mi);
    return out;
}
