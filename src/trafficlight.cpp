/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Implementation of Trafficlight class
 * */

#include "trafficlights.h"
#include "util.h"
#include "rang.hpp"
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
using namespace rang;



TrafficColor TrafficLight::getNorthSouthColor() {
    unique_lock<mutex> unqiue_lock(l_mutex);
    return this->colorNorthSouth;
}

TrafficColor TrafficLight::getWestEastColor() {
    unique_lock<mutex> unqiue_lock(l_mutex);
    return this->colorWestEast;
}

//set or control the TrafficLight: green, red or yellow and give the right outputs to the stages
void TrafficLight::startTrafficLight() {
    int counter{0};
    bool NorthSouth = false;
    bool WestEast = false;
    while(true) {
        if(counter == 2) {
            lock_guard<mutex> lock(this->l_mutex);
            logger("WEST and EAST are green");
            NorthSouth = false;
            WestEast = true;
            this->colorNorthSouth = RED;
            println("[TrafficLight] ", fg::red, "North and South Light is now RED", style::reset);
            this->colorWestEast = GREEN;
            println("[TrafficLight] ", fg::green, "West and East Light is now GREEN", style::reset);
            counter = 1;
            this_thread::sleep_for(1000ms);
        }else if(counter == 1 && NorthSouth == true) {
            lock_guard<mutex> lock(this->l_mutex);
            logger("The traffic light is yellow");
            this->colorNorthSouth = YELLOW;
            println("[TrafficLight] ", fg::yellow, "North and South Light is now YELLOW", style::reset);
            this->colorWestEast = YELLOW;
            println("[TrafficLight] ", fg::yellow, "West and East Light is now YELLOW", style::reset);
            counter++;
        }else if(counter == 1 && WestEast == true) {
            lock_guard<mutex> lock(this->l_mutex);
            logger("The traffic light is yellow");
            this->colorNorthSouth = YELLOW;
            println("[TrafficLight] ", fg::yellow, "North and South Light is now YELLOW", style::reset);
            this->colorWestEast = YELLOW;
            println("[TrafficLight] ", fg::yellow, "West and East Light is now YELLOW", style::reset);
            counter--;
        }else {
            lock_guard<mutex> lock(this->l_mutex);
            logger("NORTH and SOUTH are green");
            NorthSouth = true;
            WestEast = false;
            this->colorNorthSouth = GREEN;
            println("[TrafficLight] ", fg::green, "North and South Light is now GREEN", style::reset);
            this->colorWestEast = RED;
            println("[TrafficLight] ", fg::red, "West and East Light is now RED", style::reset);
            counter++;
            this_thread::sleep_for(100ms);
        }
        this_thread::sleep_for(5000ms);
    }
}