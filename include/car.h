/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic#
 * matNr: i15096
 * file:car.h
 * Desc: Definition of Car class and implemtation of the constructoe
 * */

#pragma once

#include "enums.h"
#include <json.hpp>
#include <iostream>

class Car
{
  private:
    std::string name;
    std::string licensePlate;
    int speed;
    Directions dir;

  public:
    static nlohmann::json generateCar(int amount, Directions direction);
  
    Car(){
        this->name = "VW";
        this->licensePlate = "WB-3456";
        this->speed = 1500;
        this->dir = NORTH;

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
    Directions getDir();
    void setDir(Directions direction);
};
