/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * matNr: i15096
 * file: street.h
 * Desc: Definition of Street class and the inline logger functions
 * */

#pragma once

#include "trafficlights.h"
#include "car.h"
#include "enums.h"
#include <iostream>
#include <queue>
#include <fstream>
#include <mutex>

class Street{
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
    Street(int generateAmount, TrafficLight *light, Directions direction, int carAmount, 
           unsigned short port, unsigned short receiverPort);
    void startStreet();
    void fillCarQueue(int amount);
    void startServer();
    int getCarAmount();
};