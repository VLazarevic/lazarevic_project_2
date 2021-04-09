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
};