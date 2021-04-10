/**
 * projectname: TrafficLight - Simulation 
 * author: Valentino Lazarevic
 * * matnr: i15096
 * file: street.cpp
 * Desc: Implementation of Street class
 * */

#include "street.h"
#include "util.h"
#include <asio.hpp>
#include <json.hpp>
#include "rang.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace rang;

//SpdLog attributes
auto err_logger = spdlog::stderr_color_mt("stderr");
auto console = spdlog::stderr_color_mt("console");

//Street constructor
Street::Street (int generateAmount, TrafficLight *light, Directions direction, int carAmount, 
                unsigned short port, unsigned short receiverPort) {
    this->light = light;
    this->direction = direction;
    this->generateAmount = generateAmount;
    this->carAmount = carAmount;
    this->port = port;
    this->receiverPort = receiverPort;
}

//Server Socket Definition
asio::io_context ioCtx;
asio::ip::tcp::socket t1SouthReadSocket(ioCtx);

int Street::getCarAmount() {
    return this->carAmount;
}

nlohmann::json getJSON(Car &tempCar) {
    nlohmann::json json;
    json["name"] = tempCar.getName();
    json["licensePlate"] = tempCar.getLicensePlate();
    json["speed"] = tempCar.getSpeed();
    json["direction"] = tempCar.getDir();

    return json;
}

Car buildFromJSON(nlohmann::json json) {

    Car newCar(json["name"], json["licensePlate"], json["speed"], json["direction"]);

    return newCar;
}

string _read(asio::ip::tcp::socket &socket) {
    asio::streambuf buf;
    asio::read_until(socket, buf, "\n");
    asio::streambuf::const_buffers_type bufs = buf.data();
    string data(asio::buffers_begin(bufs),
                asio::buffers_begin(bufs) + buf.size());

    return data;
}

int getTrafficTimer(int queueSize) {
    if (queueSize > 50) {
        return 15000;
    } if (queueSize > 25) {
        return 10000;
    } if (queueSize > 10) {
        return 7500;
    } else {
        return 5000;
    }
}

void Street::startServer() {
    if (port == 47500) {
        try {
            asio::io_context ioCtx;
            asio::error_code ec;

            string host = "127.0.0.1";

            asio::ip::address ip_address = asio::ip::make_address(host);

            asio::ip::tcp::endpoint ep(ip_address, port);
            asio::ip::tcp::acceptor acceptor(ioCtx);
            acceptor.open(ep.protocol());
            acceptor.bind(ep);
            acceptor.listen();
            acceptor.accept(t1SouthReadSocket);

            spdlog::get("console")->info("[T1 South] CONNECTION ESTABLISHED");
        } catch (exception &e) {
            spdlog::get("err_logger")->info("ERROR");
            cout << e.what() << endl;
        }
    }
}

//checking which sides are green to let the cars drive
void Street::startStreet() {

    if (port == 47500) {
        this->startServer();
        this_thread::sleep_for(1000ms);
    }

    //Create Connect Socket
    asio::io_context ioCtx;
    asio::error_code ec;
    asio::ip::tcp::socket t2NorthWriteSocket(ioCtx);

    asio::ip::address ip_address = asio::ip::make_address("127.0.0.1", ec);

    // establish connection
    if (port == 47501) {
        //connection
        t2NorthWriteSocket.connect(asio::ip::tcp::endpoint(ip_address, receiverPort), ec);

        spdlog::get("console")->info("[T2 North] Client connected to server [T1 South]");
    }

    int count = 0;
    while(true) {
        // read incoming cars
        if (port == 47500 && count > 0 && this->light->getNorthSouthColor() == GREEN) {
            try {

                spdlog::get("console")->info("[T1 South] READING");

                string save = _read(t1SouthReadSocket);
                vector<string> tokens;

                // stringstream class check1
                stringstream check1(save);

                string intermediate;

                // Tokenizing w.r.t. newline '\n'
                while (getline(check1, intermediate, '\n')) {
                    tokens.push_back(intermediate);
                }

                // Processing the token vector
                for (int i = 0; i < (int)tokens.size(); i++) {
                    nlohmann::json carJSON = nlohmann::json::parse(tokens[i]);
                    Car newCar = buildFromJSON(carJSON);
                    newCar.setDir(NORTH);
                    carQueue->push(newCar);
                    spdlog::get("console")->info("[T1 South] New car pushed to queue from Client [T2 North]! " + tokens[i]);
                }
            } catch (exception e) {
                spdlog::get("err_logger")->info("ERROR");
            }
        } if (port == 47501 && count > 0 && this->light->getNorthSouthColor() == GREEN) {
            try {

                spdlog::get("console")->info("[T2 North] READING");
                string save = _read(t2NorthWriteSocket);
                vector<string> tokens;

                // stringstream class check1
                stringstream check1(save);

                string intermediate;

                // Tokenizing w.r.t. newline '\n'
                while (getline(check1, intermediate, '\n'))
                {
                    tokens.push_back(intermediate);
                }

                // Processing the token vector
                for (int i = 0; i < (int)tokens.size(); i++)
                {
                    nlohmann::json carJSON = nlohmann::json::parse(tokens[i]);
                    Car newCar = buildFromJSON(carJSON);
                    newCar.setDir(SOUTH);
                    this->light->NorthSouthCarQueue->push(newCar);
                    spdlog::get("console")->info("[T2 North] New car pushed to queue from Client [T1 south]! " + tokens[i]);
                }
            } catch (exception e) {
                spdlog::get("err_logger")->info("ERROR");
            }
        }
        
        if ((this->direction == WEST || this->direction == EAST) && this->light->getWestEastColor() == GREEN) {
            if (!carQueue->empty()) {
                lock_guard<mutex> lock(this->l_mutex);
                Car nextCar = carQueue->front();

                if (this->direction == WEST) {
                    println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "WEST", style::reset);
                } else { 
                    println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "EAST", style::reset);
                }
                
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));

                this->light->setEast_west_timer(getTrafficTimer(carQueue->size()));

                carQueue->pop();
            }
        } else if ((this->direction == NORTH || this->direction == SOUTH) && this->light->getNorthSouthColor() == GREEN) {

            Car nextCar;
            Car NorthSouthNextCar;

            if (!carQueue->empty()) {
                nextCar = carQueue->front();
            } if(!this->light->NorthSouthCarQueue->empty()) {
                NorthSouthNextCar = this->light->getQueue()->front();
            }

            if (this->direction == NORTH) {
                // traffic light t2 north (client)
                if (port == 47501) {
                    if (!this->light->NorthSouthCarQueue->empty()) {
                        if (NorthSouthNextCar.getDir() == NORTH) {
                            // t2 north to t1 south
                            string carJSON = getJSON(NorthSouthNextCar).dump() + "\n";

                            spdlog::get("console")->info("[T2 North] Sending to Server [T1 South] JSON: " + carJSON);

                            asio::write(t2NorthWriteSocket, asio::buffer(carJSON));

                            println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, NorthSouthNextCar.getName(), " ",
                                    NorthSouthNextCar.getLicensePlate(), " drives away from ", style::bold, "T2-NORTH to T1-SOUTH",
                                    style::reset);
                            count++;
                        } else {
                            // t2 north to t2 south
                            println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, NorthSouthNextCar.getName(), " ", 
                                    NorthSouthNextCar.getLicensePlate(), " drives away from ", style::bold, "T2-NORTH to T2-SOUTH", 
                                    style::reset);   
                        }
                        this->light->NorthSouthCarQueue->pop();
                    }
                } else {
                    if (!carQueue->empty()) {
                        // t1 north to t1 south
                        string carJSON = getJSON(nextCar).dump() + "\n";

                        spdlog::get("file_logger")->info("[T1 North] Sending to Server [T1 South] JSON: " + carJSON);

                        nextCar.setDir(SOUTH);

                        this->light->NorthSouthCarQueue->push(nextCar);

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", 
                                nextCar.getLicensePlate(), " drives away from ", style::bold, "T1-NORTH to T1-SOUTH", style::reset);

                        carQueue->pop();
                    }
                }
            } else {
                // traffic light t1 south (server)
                if (port == 47500) {
                    if (!this->light->NorthSouthCarQueue->empty()) {
                        if (NorthSouthNextCar.getDir() == SOUTH) {
                            //t1 south to t2 north
                            string carJSON = getJSON(nextCar).dump() + "\n";

                            spdlog::get("console")->info("[T1 South] Sending to Server [T2 North] JSON: " + carJSON);

                            asio::write(t1SouthReadSocket, asio::buffer(carJSON));

                            println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, NorthSouthNextCar.getName(), " ", 
                                    NorthSouthNextCar.getLicensePlate(), " drives away from ", style::bold, "T1-SOUTH to T2-NORTH", 
                                    style::reset);
                            count++;
                        } else {
                            //t1 south to t1 north
                            println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, NorthSouthNextCar.getName(), " ", 
                                    NorthSouthNextCar.getLicensePlate(), " drives away from ", style::bold, "T1-SOUTH to T1-NORTH", 
                                    style::reset);
                        }
                        this->light->NorthSouthCarQueue->pop();
                    }

                } else {
                    if (!carQueue->empty()) {
                        // t2 south to t2 north
                        string carJSON = getJSON(nextCar).dump() + "\n";

                        spdlog::get("file_logger")->info("[T2 South] Sending to Server [T2 North] JSON: " + carJSON);

                        nextCar.setDir(NORTH);

                        this->light->NorthSouthCarQueue->push(nextCar);

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", 
                                nextCar.getLicensePlate(), " drives away from ", style::bold, "T2-SOUTH to T2-NORTH", style::reset);
                        carQueue->pop();
                    }
                }
            }
            this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));

            this->light->setNorth_south_timer(getTrafficTimer(carQueue->size()));
        }
    }
}

//like the function name says it fills the car queue
void Street::fillCarQueue(int amount) {
    // receive cars, fill queue => receiver
    nlohmann::json cars;
    if (this->light->getName().compare("1") == 0) {
        cars = Car::generateCar(amount, SOUTH);
    } else {
        cars = Car::generateCar(amount, NORTH);
    }

    spdlog::get("file_logger")->info("Cars are pushed in the queue");

    for (int i{0}; i < amount; i++) {
        Car tmpCar(cars[i]["name"], cars[i]["licensePlate"], cars[i]["speed"], cars[i]["direction"]);
        carQueue->push(tmpCar);
    }
}
