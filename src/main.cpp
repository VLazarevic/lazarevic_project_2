/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: The main where everything starts
 * */

#include "trafficlights.h"
#include "street.h"
#include "car.h"
#include "enums.h"
#include "CLI11.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic pop

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char* argv[]) {

    //CLI Implementation
    CLI::App app("TrafficLight-Simulation");
    
    //Commandline parameter
    int respawnTime{0};
    int amount{0};
    int carAmount{0};
    
    app.add_option("cars", amount, "How many cars after each respawn time respawns\n")->check(CLI::Number)->required();
    app.add_option("respawntime", respawnTime, "The time interval in which new cars spawn\n")->check(CLI::Number)->required();
    app.add_option("car-amount", carAmount, "The amount of cars which should be generated\n")->check(CLI::Number)->required();

    CLI11_PARSE(app, argc, argv);
    
    //set the trafficlight
    TrafficLight* light = new TrafficLight();
    //set the streets
    Street* north = new Street(amount, light, NORTH, carAmount);
    Street* east = new Street(amount, light, EAST, carAmount);
    Street* south = new Street(amount, light, SOUTH, carAmount);
    Street* west = new Street(amount, light, WEST, carAmount);

    // https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    //defining the threads
    thread lightThread(&TrafficLight::startTrafficLight, light);
    thread northStreet(&Street::startStreet, north);
    thread eastStreet(&Street::startStreet, east);
    thread southStreet(&Street::startStreet, south);
    thread westStreet(&Street::startStreet, west);

    //define the thread how is going to fill the queue
    thread carFiller([&]() {
        while(true) {
            north->fillCarQueue(carAmount);
            east->fillCarQueue(carAmount);
            south->fillCarQueue(carAmount);
            west->fillCarQueue(carAmount);
            this_thread::sleep_for(chrono::milliseconds(respawnTime * 1000));
        }
    });

    //Starting the threads
    lightThread.join();
    northStreet.join();
    eastStreet.join();
    southStreet.join();
    westStreet.join();
    carFiller.join();

    return 0;
}
