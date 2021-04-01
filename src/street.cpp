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
Street::Street(int generateAmount, TrafficLight *light, Directions direction, int carAmount, unsigned short port, unsigned short receiverPort)
{
    this->light = light;
    this->direction = direction;
    this->generateAmount = generateAmount;
    this->carAmount = carAmount;
    this->port = port;
    this->receiverPort = receiverPort;
}
/*
short Street::portInc = 1;


nlohmann::json getJSON(Car tempCar) {
    nlohmann::json json;
    json["name"] = tempCar.getName();
    json["licensePlate"] = tempCar.getLicensePlate();
    json["speed"] = tempCar.getSpeed();

    return json;
}

Car buildFromJSON(nlohmann::json json) {
    Car newCar(json["name"], json["licensePlate"], json["speed"]);

    return newCar;
}
*/
//checking which sides are green to let the cars drive
void Street::startStreet()
{
    fillCarQueue(getCarAmount());

    if (port != 0)
    {
        try
        {
            asio::io_context ioCtx;

            string host = "127.0.0.1";
            cout << "Port: " + to_string(port) << endl;
            cout << "Receive-Port: " + to_string(receiverPort) << endl;

            asio::ip::address ip_address =
                asio::ip::make_address(host);

            asio::ip::tcp::endpoint ep(ip_address, port);
            asio::ip::tcp::acceptor acceptor(ioCtx, ep);
            asio::ip::tcp::socket socket(ioCtx);
            acceptor.accept(socket);

/*
            string save = _read(socket);
            cout << save << endl;
            Car newCar;

            nlohmann::json carJSON = nlohmann::json::parse(save);

            newCar = buildFromJSON(carJSON);

            carQueue->push(newCar);

            socket.shutdown(asio::ip::tcp::socket::shutdown_both);
            socket.close();*/
        }
        catch (exception &e)
        {
            cout << e.what() << endl;
        }
    }

    while (true)
    {
        if (!carQueue->empty())
        {

            /*
            // send cars to next street => sender street
            try {
                    asio::io_context ioCtx;

                    string host = "127.0.0.1";
                    
                    string port = "1060";

                    asio::ip::tcp::resolver resolver(ioCtx.get_executor());
                    asio::ip::basic_resolver_results endpoint = resolver.resolve(host, port);

                    asio::ip::tcp::socket socket(ioCtx);
                    asio::connect(socket, endpoint);

                    asio::streambuf sender;
                    ostream send_stream(&sender);*/

            if ((this->direction == WEST || this->direction == EAST) && this->light->getWestEastColor() == GREEN)
            {
                Car nextCar = carQueue->front();

                if (this->direction == WEST)
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "WEST", style::reset);
                else
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "EAST", style::reset);
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));
                carQueue->pop();
            }
            else if ((this->direction == NORTH || this->direction == SOUTH) && this->light->getNorthSouthColor() == GREEN)
            {

                Car nextCar = carQueue->front();
                if (this->direction == NORTH)
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "NORTH", style::reset);
                else
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "SOUTH", style::reset);
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));
                
                /*sending car item
                Car tempCar = carQueue->front();
                string carJSON = getJSON(tempCar).dump();

                send_stream << carJSON;
                
                carQueue->pop();

                write(socket, sender);
                socket.shutdown(asio::ip::tcp::socket::shutdown_both);
                socket.close();*/
            }

            /*
            } catch(exception& e) {
                println(e.what());
            }*/
        }
    }
}

string _read(asio::ip::tcp::socket &socket)
{
    asio::streambuf buf;
    asio::read_until(socket, buf, "\n");

    while (asio::read(socket, buf,
                      asio::transfer_at_least(1)))
    {
        cout << &buf << endl;
    }

    asio::streambuf::const_buffers_type bufs = buf.data();
    string data(asio::buffers_begin(bufs),
                asio::buffers_begin(bufs) + buf.size());

    return data;
}

void _send(asio::ip::tcp::socket &socket, const string &message)
{
    asio::write(socket, asio::buffer(message));
}

//like the function name says it fills the car queue
void Street::fillCarQueue(int amount)
{
    // receive cars, fill queue => receiver
    /*
    if(isFirstStreet) {*/
    nlohmann::json cars = Car::generateCar(amount);

    logger("Cars are pushed in the queue");
    for (int i{0}; i < amount; i++)
    {
        Car tmpCar(cars[i]["name"], cars[i]["licensePlate"], cars[i]["speed"]);
        carQueue->push(tmpCar);
    }
    /*
        isFirstStreet = false;
    } else {

        try {
            asio::io_context ioCtx;

            string host = "127.0.0.1";
            short port = 1025 + ++portInc;
            cout << port << endl;
            

            asio::ip::address ip_address =
            asio::ip::make_address(host);

            asio::ip::tcp::endpoint ep(ip_address, port);

            asio::ip::tcp::acceptor acceptor(ioCtx, ep);


            asio::ip::tcp::socket socket(ioCtx);
            acceptor.accept(socket);
        

            string save = _read(socket);

            cout << save << endl;

            Car newCar;

            nlohmann::json carJSON = nlohmann::json::parse(save);

            newCar = buildFromJSON(carJSON);

            carQueue->push(newCar);

            socket.shutdown(asio::ip::tcp::socket::shutdown_both);
            socket.close();
        } catch(exception& e) {
            cout << e.what() << endl;
        }

        
    }*/
}

void getCarsFromJSON()
{
    return;
}
