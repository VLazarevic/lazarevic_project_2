/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Definition of Car class and implemtation of the constructoe
 * */


#pragma once

#include <iostream>

class Car
{
private:
    std::string name;
    std::string licensePlate;
    int speed;
public:
    static Car generateCar();
    
    Car(std::string name, std::string licensePlate, int speed) {
        this->name = name;
        this->licensePlate = licensePlate;
        this->speed = speed;
}

    std::string getLicensePlate();
    std::string getName();
    int getSpeed();
};
