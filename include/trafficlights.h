/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Definition of trafficlight class and the inline logger functions
 * */


#pragma once

#include "enums.h"
#include <iostream>
#include <mutex>
#include <fstream>

class TrafficLight {
    private:
        TrafficColor colorNorthSouth;
        TrafficColor colorWestEast;
        std::mutex l_mutex;
        std::string name;
        int north_south_timer;
        int east_west_timer;

    public:
        TrafficColor getNorthSouthColor();
        TrafficColor getWestEastColor();
        std::string getName();
        void setNorth_south_timer(int timer);
        void setEast_west_timer(int timer);
        void startTrafficLight();
        TrafficLight(std::string name);

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