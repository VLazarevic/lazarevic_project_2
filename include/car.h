/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Definition of Car class and implemtation of the constructoe
 * */

#pragma once

#include <iostream>
#include <json.hpp>
#include "enums.h"

class Car
{
  private:
    std::string name;
    std::string licensePlate;
    int speed;
    int direction;

  public:
    
    static nlohmann::json generateCar(int amount);

    Car(){
        this->name = "";
        this->licensePlate = "";
        this->speed = 0;
        this->direction = 0;
    }

    Car(std::string name, std::string licensePlate, int speed, int direction){
        this->name = name;
        this->licensePlate = licensePlate;
        this->speed = speed;
        this->direction = direction;
    }

    std::string getLicensePlate();
    std::string getName();
    int getSpeed();
    int getDirection();
    void setDirection(int direction);
};
