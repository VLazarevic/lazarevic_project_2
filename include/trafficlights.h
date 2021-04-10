/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * matNr: i15096
 * file: trafficlights.h
 * Desc: Definition of trafficlight class and the inline logger functions
 * */


#pragma once

#include "enums.h"
#include "car.h"
#include <iostream>
#include <mutex>
#include <fstream>
#include <queue>

class TrafficLight {
  private:
    TrafficColor colorNorthSouth;
    TrafficColor colorWestEast;
    std::mutex l_mutex;
    std::string name;
    int north_south_timer;
    int east_west_timer;
    int counter; 
     
  public:
    std::queue<Car> *NorthSouthCarQueue = new std::queue<Car>();
    TrafficColor getNorthSouthColor();
    TrafficColor getWestEastColor();
    std::string getName();
    void setNorth_south_timer(int timer);
    void setEast_west_timer(int timer);
    void startTrafficLight();
    TrafficLight(std::string name, int counter);
    std::queue<Car> *getQueue(); 

};