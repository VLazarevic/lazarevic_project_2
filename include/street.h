/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Definition of Street class and the inline logger functions
 * */


#pragma once

#include "trafficlights.h"
#include "car.h"
#include "enums.h"

#include <iostream>
#include <queue>
#include <fstream>

class Street
{
private:
    TrafficLight* light;
    Directions direction;
    int generateAmount;
    std::queue<Car>* carQueue = new std::queue<Car>();
public:
    Street(int generateAmount, TrafficLight* light, Directions direction);
    void startStreet();
    void fillCarQueue();

    inline std::string getCurrentTime(std::string s){
            time_t now = time(0);
            struct tm tstruct;
            char buf[80];
            tstruct = *localtime(&now);
            if(s=="now"){
                strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
            }else if(s=="date"){
                strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
            }
            return std::string(buf);
        }

    inline void logger(std::string logMsg){
        std::string filePath = "../log/log_" + getCurrentTime("date") + ".txt";
        std::string now = getCurrentTime("now");
        std::fstream ofs;
            ofs.open(filePath, std::ios_base::out | std::ios_base::app );
            if(!ofs.good()){
                std::cout << "Logging has a problem!";
            }
        ofs << now << '\t' << logMsg << '\n' << std::flush;
        ofs.close();
    }
};