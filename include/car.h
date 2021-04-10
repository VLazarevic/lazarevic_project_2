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

  public:
    Directions dir;
    static nlohmann::json generateCar(int amount, Directions direction);

    Car(){
        this->name = "";
        this->licensePlate = "";
        this->speed = 0;
    }

    Car(std::string name, std::string licensePlate, int speed, Directions dir){
        this->name = name;
        this->licensePlate = licensePlate;
        this->speed = speed;
        this->dir = dir;
    }

    std::string getLicensePlate();
    std::string getName();
    int getSpeed();
};
