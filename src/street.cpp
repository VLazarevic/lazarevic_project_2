/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Implementation of Street class
 * */

#include "street.h"
#include "util.h"
#include "rang.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace rang;

//Street constructor
Street::Street(int generateAmount, TrafficLight* light, Directions direction) {
    this->light = light;
    this->direction = direction;
    this->generateAmount = generateAmount;
}

//checking which sides are green to let the cars drive
void Street::startStreet() {
    fillCarQueue();
    while(true) {
        if(!carQueue->empty()) {
            if((this->direction == WEST || this->direction == EAST) && this->light->getWestEastColor() == GREEN) {
                Car nextCar = carQueue->front();
                if(this->direction == WEST)
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "WEST", style::reset);
                else
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "EAST", style::reset);
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));
                carQueue->pop();
            } else if((this->direction == NORTH || this->direction == SOUTH) && this->light->getNorthSouthColor() == GREEN) {
                Car nextCar = carQueue->front();
                if(this->direction == NORTH)
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "NORTH", style::reset);
                else
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "SOUTH", style::reset);
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));
                carQueue->pop();
            }
        }
    }   
}

//like the function name says it fills the car queue
void Street::fillCarQueue() {
    logger("Cars are pushed in the queue");
    for(int i{0}; i < this->generateAmount; i++) {
        carQueue->push(Car::generateCar());
    }
}
