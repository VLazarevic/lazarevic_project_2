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
#include <vector>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace rang;

//SpdLog attributes
auto err_logger = spdlog::stderr_color_mt("stderr");
auto console = spdlog::stderr_color_mt("console");

//Street constructor
Street::Street(int generateAmount, TrafficLight *light, Directions direction, int carAmount, unsigned short port, unsigned short receiverPort)
{
    this->light = light;
    this->direction = direction;
    this->generateAmount = generateAmount;
    this->carAmount = carAmount;
    this->port = port;
    this->receiverPort = receiverPort;
}

asio::io_context ioCtx;
asio::ip::tcp::socket t1SouthReadSocket(ioCtx);
//asio::ip::tcp::socket t2NorthReadSocket(ioCtx);

nlohmann::json getJSON(Car tempCar)
{
    nlohmann::json json;
    json["name"] = tempCar.getName();
    json["licensePlate"] = tempCar.getLicensePlate();
    json["speed"] = tempCar.getSpeed();
    json["direction"] = tempCar.dir;

    return json;
}

Car buildFromJSON(nlohmann::json json)
{
    Car newCar(json["name"], json["licensePlate"], json["speed"]);

    return newCar;
}

string _read(asio::ip::tcp::socket &socket)
{
    asio::streambuf buf;
    asio::read_until(socket, buf, "\n");
    asio::streambuf::const_buffers_type bufs = buf.data();
    string data(asio::buffers_begin(bufs),
                asio::buffers_begin(bufs) + buf.size());

    return data;
}

int getTrafficTimer(int queueSize)
{
    if (queueSize > 50)
    {
        return 15000;
    }
    if (queueSize > 25)
    {
        return 10000;
    }
    if (queueSize > 10)
    {

        return 7500;
    }
    else
    {
        return 5000;
    }
}

void Street::startServer()
{
    if (port == 47500)
    {
        try
        {
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

            //this_thread::sleep_for(2000ms);

            //t2NorthSocket.connect(asio::ip::tcp::endpoint(ip_address, 47501), ec);

            spdlog::get("console")->info("[T1 South] CONNECTION ESTABLISHED");
        }
        catch (exception &e)
        {
            spdlog::get("err_logger")->info("ERROR");
            cout << e.what() << endl;
        }
    } /*
    if (port == 47501)
    {
        try
        {
            asio::io_context ioCtx;
            asio::error_code ec;

            string host = "127.0.0.1";

            v

            spdlog::get("system")->info("[T2 North] CONNECTION ESTABLISHED");

            //t2NorthSocket.connect(asio::ip::tcp::endpoint(ip_address, ), ec);
            //spdlog::get("file_logger")->info("[to T1 South] CONNECTION ESTABLISHED" + ec.message()); 
        }
        catch (exception &e)
        {
            spdlog::get("err_logger")->info("ERROR");
            cout << e.what() << endl;
        }
    }*/
}

//checking which sides are green to let the cars drive
void Street::startStreet()
{

    if (port == 47500 /*|| port == 47501*/)
    {
        this->startServer();
        this_thread::sleep_for(1000ms);
    }

    asio::io_context ioCtx;
    //asio::io_context tmpioCtx;

    asio::error_code ec;

    //socket creation
    asio::ip::tcp::socket t2NorthWriteSocket(ioCtx);
    //asio::ip::tcp::socket t1SouthWriteSocket(tmpioCtx);

    asio::ip::address ip_address = asio::ip::make_address("127.0.0.1", ec);

    // establish connection

    if (port == 47501)
    {

        //connection
        t2NorthWriteSocket.connect(asio::ip::tcp::endpoint(ip_address, receiverPort), ec);

        spdlog::get("console")->info("[T2 North] Client connected to server [T1 South]");
    }

    int count = 0;
    while (true)
    {

        // read incoming cars
        if (port == 47500 && count > 0)
        {
            try
            {

                spdlog::get("console")->info("[T1 South] READING");

                string save = _read(t1SouthReadSocket);
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
                for (int i = 0; i < (int)tokens.size(); i++){
                    nlohmann::json carJSON = nlohmann::json::parse(tokens[i]);
                    Car newCar = buildFromJSON(carJSON);
                    carQueue->push(newCar);
                    spdlog::get("console")->info("[T1 South] New car pushed to queue from Client [T2 North]! " + save);
                } 
            }
            catch (exception e)
            {
                spdlog::get("err_logger")->info("ERROR");
            }
        }
        if (port == 47501 && count > 0)
        {
            try
            {

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
                for (int i = 0; i < (int)tokens.size(); i++){
                    nlohmann::json carJSON = nlohmann::json::parse(tokens[i]);
                    Car newCar = buildFromJSON(carJSON);
                    carQueue->push(newCar);
                    spdlog::get("console")->info("[T1 South] New car pushed to queue from Client [T2 North]! " + save);
                } 
            }
            catch (exception e)
            {
                spdlog::get("err_logger")->info("ERROR");
            }
        }

        if (!carQueue->empty())
        {

            if ((this->direction == WEST || this->direction == EAST) && this->light->getWestEastColor() == GREEN)
            {
                /*
                lock_guard<mutex> lock(this->l_mutex);
                Car nextCar = carQueue->front();

                if (this->direction == WEST)
                    println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "WEST", style::reset);
                else
                    println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "EAST", style::reset);
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));

                this->light->setEast_west_timer(getTrafficTimer(carQueue->size()));

                carQueue->pop();*/
            }
            else if ((this->direction == NORTH || this->direction == SOUTH) && this->light->getNorthSouthColor() == GREEN)
            {
                lock_guard<mutex> lock(this->l_mutex);

                Car nextCar = carQueue->front();
                if (this->direction == NORTH)
                {
                    // traffic light t2 north (client)
                    if (port == 47501)
                    {
                        //sending car item

                        // t2 north to t1 south
                        nextCar.dir = NORTH;
                        string carJSON = getJSON(nextCar).dump() + "\n";

                        spdlog::get("file_logger")->info("[T2 North] Sending to Server [T1 South] JSON: " + carJSON);

                        asio::write(t2NorthWriteSocket, asio::buffer(carJSON));

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(),
                                " drives away from ", style::bold, "T2-NORTH to T1-SOUTH", style::reset);
                        count++;
                    }
                    else
                    {
                        /*
                        // t1 north to t1 south
                        string carJSON = getJSON(nextCar).dump() + "\n";

                        spdlog::get("file_logger")->info("[T1 North] Sending to Server [T1 South] JSON: " + carJSON);

                        //asio::write(t1NorthSocket, asio::buffer(carJSON));

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), 
                                " drives away from ", style::bold, "T1-NORTH to T1-SOUTH", style::reset);

                                */
                    }

                    carQueue->pop();
                }
                else
                {
                    // traffic light t1 south

                    if (port == 47500)
                    {
                        //sending car item

                        // t1south to t2north
                        string carJSON = getJSON(nextCar).dump() + "\n";

                        spdlog::get("file_logger")->info("[T1 South] Sending to Server [T2 North] JSON: " + carJSON);

                        asio::write(t1SouthReadSocket, asio::buffer(carJSON));

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(),
                                " drives away from ", style::bold, "T1-SOUTH to T2-NORTH", style::reset);
                        count++;
                    }
                    else
                    {
                        /*
                        // t2 south to t2 north
                        string carJSON = getJSON(nextCar).dump() + "\n";
                        
                        spdlog::get("file_logger")->info("[T2 South] Sending to Server [T2 North] JSON: " + carJSON);

                        
                        //asio::write(t2SouthSocket, asio::buffer(carJSON));

                        println("[Trafficlight ", this->light->getName(), "][CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), 
                                " drives away from ", style::bold, "T2-SOUTH to T2-NORTH", style::reset);
                                */
                    }
                    carQueue->pop();
                }
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));

                this->light->setNorth_south_timer(getTrafficTimer(carQueue->size()));
            }
        }
    }
}

void _send(asio::ip::tcp::socket &socket, const string &message)
{
    asio::write(socket, asio::buffer(message));
}

//like the function name says it fills the car queue
void Street::fillCarQueue(int amount)
{
    // receive cars, fill queue => receiver

    nlohmann::json cars = Car::generateCar(amount);
    spdlog::get("file_logger")->info("Cars are pushed in the queue");

    for (int i{0}; i < amount; i++)
    {
        Car tmpCar(cars[i]["name"], cars[i]["licensePlate"], cars[i]["speed"]);
        carQueue->push(tmpCar);
    }
}
