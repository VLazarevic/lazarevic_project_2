/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Definition of Street class and the inline logger functions
 * */

#pragma once

#include "trafficlights.h"
#include "car.h"
#include "enums.h"
#include <asio.hpp>
#include <iostream>
#include <queue>
#include <fstream>
#include <mutex>

//#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <spdlog/spdlog.h>


class Street
{
private:
    TrafficLight *light;
    Directions direction;
    int generateAmount;
    int carAmount;
    unsigned short port;
    unsigned short receiverPort;
    std::queue<Car> *carQueue = new std::queue<Car>();
    //int count = 0;

    std::mutex l_mutex;

public:
    Street(int generateAmount, TrafficLight *light, Directions direction, int carAmount, unsigned short port, unsigned short receiverPort);
    void startStreet();
    void fillCarQueue(int amount);
    void connect();
    void startServer();
    int getCarAmount()
    {
        return this->carAmount;
    }

    inline std::string getCurrentTime(std::string s)
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        if (s == "now")
        {
            strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        }
        else if (s == "date")
        {
            strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        }
        return std::string(buf);
    }

    inline void logger(std::string logMsg)
    {
        
        std::string filePath = "../log/log_" + getCurrentTime("date") + ".txt";
        std::string now = getCurrentTime("now");
        std::fstream ofs;
        ofs.open(filePath, std::ios_base::out | std::ios_base::app);
        if (!ofs.good())
        {
            std::cout << "Logging has a problem!";
        }
        ofs << now << '\t' << logMsg << '\n'
            << std::flush;
        ofs.close();
        /*
        try
        {
            count++;
            auto max_size = 1048576 * 5;
            auto max_files = 3;
            auto logger = spdlog::rotating_logger_mt("Logger" + std::to_string(count), "../log/log_" + getCurrentTime("date") + ".txt", max_size, max_files);

            spdlog::get("Logger" + std::to_string(count))->info(logMsg);
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            std::cout << "Log init failed: " << ex.what() << std::endl;
        }*/
    }
};