/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Car class where the logic of the functions are implemented
 * */

#include "car.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

//generate a random string, that get used by defining the license plate
string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    string str(length,0);
    generate_n( str.begin(), length, randchar );
    return str;
}

//generate the car, with the attributes: name, licensePlate, speed
Car Car::generateCar() {
    random_device randomDevice;
    mt19937 gen{randomDevice()};
    
    vector<string> districtTypes = {"AM", "BN", "BL", "GD", "GF", "HL", "HO", "KO", "KR", "KS", "LF", "MD", "ME", "MI", "NK", "P", "SP", 
                                    "SB", "SW", "TU", "WN", "WB", "WT", "WY", "ZT"};
    uniform_real_distribution<> dis{0, (double) districtTypes.size()};

    string licensePlate = districtTypes[(int) dis(gen)] + "-" + random_string(5);

    vector<string> carTypes = {"BMW", "VW", "AUDI", "MERCEDES", "PORSCHE", "OPEL", "NISSAN", "MAZDA", "TOYOTA", "HONDA", "PEUGEOT"};

    uniform_real_distribution<> dis_2{0, (double) carTypes.size()};
    string name = carTypes[(int) dis_2(gen)];

    // miliseconds
    uniform_real_distribution<> dis_3{1000, 2500};

    int speed = (int) dis_3(gen);

    return Car(name, licensePlate, speed);
}

std::string Car::getLicensePlate() {
    return this->licensePlate;
}

std::string Car::getName() {
    return this->name;
}

int Car::getSpeed() {
    return this->speed;
}