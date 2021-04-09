/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * matnr: i15096
 * file: car.cpp
 * Desc: Car class where the logic of the functions are implemented
 * */

#include "car.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include <json.hpp>

using namespace std;

//generate a random string, that get used by defining the license plate
string random_string( size_t length ){
    auto randchar = []() -> char{
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
nlohmann::json Car::generateCar(int amount) {

    // JSON
    nlohmann::json cars = {};

    for(int i{0}; i < amount; i++) {
        nlohmann::json car;
        random_device randomDevice;
        mt19937 gen{randomDevice()};
        
        vector<string> districtTypes = {"AM", "BN", "BL", "GD", "GF", "HL", "HO", "KO", "KR", "KS", "LF", "MD", "ME",
                                        "MI", "NK", "P", "SP", "SB", "SW", "TU", "WN", "WB", "WT", "WY", "ZT"};
        uniform_real_distribution<> dis{0, (double) districtTypes.size()};

        string licensePlate = districtTypes[(int) dis(gen)] + "-" + random_string(5);

        vector<string> carTypes = {"BMW", "VW", "AUDI", "MERCEDES", "PORSCHE", "OPEL", "NISSAN", "MAZDA", "TOYOTA", "HONDA", "PEUGEOT"};

        uniform_real_distribution<> dis_2{0, (double) carTypes.size()};
        string name = carTypes[(int) dis_2(gen)];

        // miliseconds
        uniform_real_distribution<> dis_3{1000, 2500};

        int speed = (int) dis_3(gen);
        car["name"] = name;
        car["licensePlate"] = licensePlate;
        car["speed"] = speed;

        cars.push_back(car);
    }

    //write a JSON with cars
    fstream carStream;

    // Clearing file
    carStream.open("../doc/json/cars.json", std::ofstream::out | std::ofstream::trunc);
    carStream.close();

    carStream.open("../doc/json/cars.json");

    carStream << cars;

    carStream.close();

    return cars;
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



