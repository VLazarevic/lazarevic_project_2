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
    TrafficLight* trafficLight1 = new TrafficLight();
    TrafficLight* trafficLight2 = new TrafficLight();
    
    //set the streets
    Street* t1North = new Street(amount, trafficLight1, NORTH, carAmount);
    Street* t1East = new Street(amount, trafficLight1, EAST, carAmount);
    Street* t1South = new Street(amount, trafficLight1, SOUTH, carAmount, 47500, 47501);
    Street* t1West = new Street(amount, trafficLight1, WEST, carAmount);

    Street* t2North = new Street(amount, trafficLight2, NORTH, carAmount, 47501, 47500);
    Street* t2East = new Street(amount, trafficLight2, EAST, carAmount);
    Street* t2South = new Street(amount, trafficLight2, SOUTH, carAmount);
    Street* t2West = new Street(amount, trafficLight2, WEST, carAmount);


    // https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    //defining the threads
    thread t1Thread(&TrafficLight::startTrafficLight, trafficLight1);
    thread t1NorthStreet(&Street::startStreet, t1North);
    thread t1EastStreet(&Street::startStreet, t1East);
    thread t1SouthStreet(&Street::startStreet, t1South);
    thread t1WestStreet(&Street::startStreet, t1West);

    thread t2Thread(&TrafficLight::startTrafficLight, trafficLight2);
    thread t2NorthStreet(&Street::startStreet, t2North);
    thread t2EastStreet(&Street::startStreet, t2East);
    thread t2SouthStreet(&Street::startStreet, t2South);
    thread t2WestStreet(&Street::startStreet, t2West);


    //define the thread which is going to fill the queue
    thread carFiller([&]() {
        while(true) {
            t1North->fillCarQueue(carAmount);
            t1East->fillCarQueue(carAmount);
            //t1South->fillCarQueue(carAmount);
            t1West->fillCarQueue(carAmount);
            //t2North->fillCarQueue(carAmount);
            t2East->fillCarQueue(carAmount);
            t2South->fillCarQueue(carAmount);
            t2West->fillCarQueue(carAmount);
            this_thread::sleep_for(chrono::milliseconds(respawnTime * 1000));
        }
    });

    //Starting the threads
    t1Thread.join();
    t1NorthStreet.join();
    t1EastStreet.join();
    t1SouthStreet.join();
    t1WestStreet.join();
    t2Thread.join();
    t2NorthStreet.join();
    t2EastStreet.join();
    t2SouthStreet.join();
    t2WestStreet.join();
    carFiller.join();

    return 0;
}
