/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * Desc: Implementation of Street class
 * */

#include <asio.hpp>
#include <json.hpp>
#include "street.h"
#include "util.h"
#include "rang.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace rang;


bool isFirstStreet = true;

//Street constructor
Street::Street(int generateAmount, TrafficLight* light, Directions direction) {
    this->light = light;
    this->direction = direction;
    this->generateAmount = generateAmount;
}

nlohmann::json getJSON(Car tempCar) {
    nlohmann::json json;
    json["name"] = tempCar.getName();
    json["licensePlate"] = tempCar.getLicensePlate();
    json["speed"] = tempCar.getSpeed();

    return json;
}

//checking which sides are green to let the cars drive
void Street::startStreet() {
    fillCarQueue();
    while(true) {
        if(!carQueue->empty()) {

            

            // send cars to next street => sender street
            try {
                
                //asio definiton

                asio::io_context ioCtx;

                string host = "187.0.0.1";
                string port = "50000";

                asio::ip::tcp::resolver resolver(ioCtx.get_executor());
                asio::ip::basic_resolver_results endpoint = resolver.resolve(host, port);

                asio::ip::tcp::socket socket(ioCtx);
                asio::connect(socket, endpoint);

                asio::streambuf sender;
                ostream send_stream(&sender);

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

                    //sending car item
                    Car tempCar = carQueue->front();
                    string carJSON = getJSON(tempCar).dump();

                    send_stream << carJSON;

                    carQueue->pop();

                }

                write(socket, sender);
            } catch(exception e) {
                println("error");
            }
        }
    }   
}

//like the function name says it fills the car queue
void Street::fillCarQueue() {
    // receive cars, fill queue => receiver

    if(isFirstStreet) {
        // getCarsFromJSON();
        isFirstStreet = false;
    } else {
        asio::io_context ioCtx;

        string host = "187.0.0.1";
        string port = "50000";

        asio::ip::tcp::resolver resolver(ioCtx.get_executor());
        asio::ip::basic_resolver_results endpoint = resolver.resolve(host, port);

        asio::ip::tcp::socket socket(ioCtx);
        asio::connect(socket, endpoint);

        asio::streambuf receiver;
        istream receiver_stream(&receiver);

        Car newCar;
        

        string save;
        receiver_stream >> save;

        nlohmann::json carJSON = nlohmann::json::parse(save);

        //newCar.buildFromJSON(carJSON);
    }

    logger("Cars are pushed in the queue");
    for(int i{0}; i < this->generateAmount; i++) {
        carQueue->push(Car::generateCar());
    }
}

void getCarsFromJSON() {
    return;
}