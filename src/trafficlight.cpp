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
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace rang;


TrafficLight::TrafficLight(string name, int counter){
    this->name = name;
    this->north_south_timer = 5000;
    this->east_west_timer = 5000;
    this->counter = counter;
}

TrafficColor TrafficLight::getNorthSouthColor() {
    unique_lock<mutex> unqiue_lock(l_mutex);
    return this->colorNorthSouth;
}

TrafficColor TrafficLight::getWestEastColor() {
    unique_lock<mutex> unqiue_lock(l_mutex);
    return this->colorWestEast;
}

string TrafficLight::getName(){
    unique_lock<mutex> unqiue_lock(l_mutex);
    return this->name;
}

void TrafficLight::setNorth_south_timer(int timer){
    unique_lock<mutex> unqiue_lock(l_mutex);
    this->north_south_timer = timer;
}


void TrafficLight::setEast_west_timer(int timer){
    unique_lock<mutex> unqiue_lock(l_mutex);
    this->east_west_timer = timer;
}

//auto file_logger = spdlog::basic_logger_mt("traffic_file_logger", "trafficlight_logger.txt");
auto file_logger = spdlog::basic_logger_mt("file_logger", "logger.txt");
//set or control the TrafficLight: green, red or yellow and give the right outputs to the stages
void TrafficLight::startTrafficLight() {
    //int counter{0};
    bool NorthSouth = false;
    bool WestEast = false;
    while(true) {
        if(counter == 2) {
            this->l_mutex.lock();
            spdlog::get("file_logger")->info("WEST and EAST are green");
            NorthSouth = false;
            WestEast = true;
            this->colorNorthSouth = RED;
            println("[TrafficLight " + name + "] ", fg::red, "North and South Light is now RED", style::reset);
            this->colorWestEast = GREEN;
            println("[TrafficLight " + name +  "] ", fg::green, "West and East Light is now GREEN", style::reset);
            counter = 1;
            this->l_mutex.unlock();
            this_thread::sleep_for(chrono::milliseconds(east_west_timer));
        }else if(counter == 1 && NorthSouth == true) {
            this->l_mutex.lock();
            spdlog::get("file_logger")->info("The traffic light is yellow");
            this->colorNorthSouth = YELLOW;
            println("[TrafficLight " + name +  "] ", fg::yellow, "North and South Light is now YELLOW", style::reset);
            this->colorWestEast = YELLOW;
            println("[TrafficLight " + name +  "] ", fg::yellow, "West and East Light is now YELLOW", style::reset);
            counter++;
            this->l_mutex.unlock();
            this_thread::sleep_for(2500ms);
        }else if(counter == 1 && WestEast == true) {
            this->l_mutex.lock();
            spdlog::get("file_logger")->info("The traffic light is yellow");
            this->colorNorthSouth = YELLOW;
            println("[TrafficLight " + name +  "] ", fg::yellow, "North and South Light is now YELLOW", style::reset);
            this->colorWestEast = YELLOW;
            println("[TrafficLight " + name +  "] ", fg::yellow, "West and East Light is now YELLOW", style::reset);
            counter--;
            this->l_mutex.unlock();
            this_thread::sleep_for(2500ms);
        }else {
            this->l_mutex.lock();
            spdlog::get("file_logger")->info("NORTH and SOUTH are green");
            NorthSouth = true;
            WestEast = false;
            this->colorNorthSouth = GREEN;
            println("[TrafficLight " + name +  "] ", fg::green, "North and South Light is now GREEN", style::reset);
            this->colorWestEast = RED;
            println("[TrafficLight " + name +  "] ", fg::red, "West and East Light is now RED", style::reset);
            counter++;
            this->l_mutex.unlock();
            this_thread::sleep_for(chrono::milliseconds(north_south_timer));
        }
    }
}