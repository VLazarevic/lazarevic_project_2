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
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace std;
using namespace rang;

//SpdLog attributes
auto console = spdlog::stdout_color_mt("console");
auto err_logger = spdlog::stderr_color_mt("stderr");

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
asio::ip::tcp::socket streetSocket(ioCtx);
asio::ip::tcp::socket tmpSocket(ioCtx);
/*
short Street::portInc = 1;
*/

nlohmann::json getJSON(Car tempCar)
{
    nlohmann::json json;
    json["name"] = tempCar.getName();
    json["licensePlate"] = tempCar.getLicensePlate();
    json["speed"] = tempCar.getSpeed();

    return json;
}

Car buildFromJSON(nlohmann::json json)
{
    Car newCar(json["name"], json["licensePlate"], json["speed"]);

    return newCar;
}

string _read(asio::ip::tcp::socket &socket)
{
    /*
    asio::streambuf buf;
    
    asio::read_until(socket, buf, "\n");

    while (asio::read(socket, buf,
                      asio::transfer_at_least(1)))
    {
        cout << &buf << endl;
    }
    */

    asio::streambuf buf;
    asio::read_until(socket, buf, "\n");
    asio::streambuf::const_buffers_type bufs = buf.data();
    string data(asio::buffers_begin(bufs),
                asio::buffers_begin(bufs) + buf.size());
    //string data = asio::buffer_cast<const char*>(buf.data());

    return data;
}

void Street::startServer()
{
    if (port == 47500)
    {
        try
        {
            asio::io_context ioCtx;

            string host = "127.0.0.1";
            cout << "Port: " + to_string(port) << endl;
            cout << "Receive-Port: " + to_string(receiverPort) << endl;

            asio::ip::address ip_address = asio::ip::make_address(host);

            asio::ip::tcp::endpoint ep(ip_address, port);
            asio::ip::tcp::acceptor acceptor(ioCtx);
            acceptor.open(ep.protocol());
            acceptor.bind(ep);
            acceptor.listen();
            //asio::ip::tcp::socket socket(ioCtx);
            //this->socket = new asio::ip::tcp::socket(ioCtx);
            acceptor.accept(streetSocket);

            spdlog::get("console")->info("[T1 South] CONNECTION ESTABLISHED");

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
            spdlog::get("err_logger")->info("ERROR");
            cout << e.what() << endl;
        }
    }
    if (port == 47501)
    {
        try
        {
            asio::io_context ioCtx;

            string host = "127.0.0.1";
            cout << "Port: " + to_string(port) << endl;
            cout << "Receive-Port: " + to_string(receiverPort) << endl;

            asio::ip::address ip_address = asio::ip::make_address(host);

            asio::ip::tcp::endpoint ep(ip_address, port);
            asio::ip::tcp::acceptor acceptor(ioCtx);
            acceptor.open(ep.protocol());
            acceptor.bind(ep);
            acceptor.listen();
            //asio::ip::tcp::socket socket(ioCtx);
            //this->socket = new asio::ip::tcp::socket(ioCtx);
            acceptor.accept(tmpSocket);

            spdlog::get("console")->info("[T2 North] CONNECTION ESTABLISHED");

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
            spdlog::get("err_logger")->info("ERROR");
            cout << e.what() << endl;
        }
    }
}

//checking which sides are green to let the cars drive
void Street::startStreet()
{
    //asio::ip::tcp::socket *streetSocket;
    //fillCarQueue(getCarAmount());

    if (port == 47500 || port == 47501)
    {
        this->startServer();
    }

    //cout << "Port: " + to_string(port) + " ReceiverPort: " + to_string(receiverPort) << endl;

    asio::io_context ioCtx;
    asio::io_context tmpioCtx;

    asio::error_code ec;
    //socket creation
    asio::ip::tcp::socket socket(ioCtx);
    asio::ip::tcp::socket tmpsocket(tmpioCtx);

    asio::ip::address ip_address = asio::ip::make_address("127.0.0.1", ec);

    // establish connection
    if (port == 47499)
    {

        /*
        asio::ip::tcp::endpoint ep(ip_address, port);
        asio::ip::tcp::acceptor acceptor(ioCtx);
        cout << ec.message() <<endl;
        acceptor.open(ep.protocol());
        acceptor.bind(ep);
        acceptor.listen();
        //asio::ip::tcp::socket socket(ioCtx);
        //this->socket = new asio::ip::tcp::socket(ioCtx);
        acceptor.accept(socket);
        spdlog::get("console")->info("[T2 North] CONNECTION ESTABLISHED");*/
        //connection
        socket.connect(asio::ip::tcp::endpoint(ip_address, receiverPort), ec);
        //cout << ec.message() <<endl;
        // request/message from client
        //const string msg = "Hello from Client!\n";
        //asio::write(socket, asio::buffer(msg));
        spdlog::get("console")->info("[T2 North] Client connected to server [T1 South]");

        // getting response from server
        //this_thread::sleep_for(chrono::milliseconds(1000));
        //asio::streambuf receive_buffer;

        //string response = _read(streetSocket);
    }
    if (port == 47502)
    {

        tmpsocket.connect(asio::ip::tcp::endpoint(ip_address, receiverPort), ec);
        spdlog::get("console")->info("[T2 South] Client connected to server [T2 North]");

        //spdlog::get("err_logger")->info(ec.message());
        //spdlog::get("console")->info("[T2 South] Client connected to server [T2 North]");
    }

    while (true)
    {

        // read incoming cars
        if (port == 47500)
        {
            try
            {

                spdlog::get("console")->info("[T1 South] READING");
                string save = _read(streetSocket);
                nlohmann::json carJSON = nlohmann::json::parse(save);
                Car newCar = buildFromJSON(carJSON);
                carQueue->push(newCar);
                spdlog::get("console")->info("[T1 South] New car pushed to queue from Client [T1 North]! " + save + "[" + to_string(carQueue->size()) + "]");
            }
            catch (exception e)
            {
                spdlog::get("err_logger")->info("ERROR");
            }
        }
        if (port == 47501)
        {
            try
            {

                spdlog::get("console")->info("[T2 North] READING");
                string save = _read(tmpSocket);
                nlohmann::json carJSON = nlohmann::json::parse(save);
                Car newCar = buildFromJSON(carJSON);
                carQueue->push(newCar);
                spdlog::get("console")->info("[T2 North] New car pushed to queue from Client [T2 South]! " + save + "[" + to_string(carQueue->size()) + "]");
            }
            catch (exception e)
            {
                spdlog::get("err_logger")->info("ERROR");
            }
        }

        //cout << "Port: " + to_string(port) + " ReceiverPort: " + to_string(receiverPort) << endl;

        //int counter = 0;
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
                {
                    // traffic light t2 north (client)
                    if (port == 47501)
                    {
                        //sending car item
                        /*
                        Car tempCar = carQueue->front();
                        string carJSON = getJSON(tempCar).dump();

                        send_stream << carJSON;

                        carQueue->pop();
                        write(socket, sender);*/
                        //sending car item
                        //Car tempCar = carQueue->front();
                        /*
                        nextCar.direction = NORTH;
                        string carJSON = getJSON(nextCar).dump() + "\n";
                        //send_stream << carJSON;
                        spdlog::get("console")->info("[T2 North] Sending to Server [T1 South] JSON: " + carJSON);

                        this_thread::sleep_for(chrono::milliseconds(1000));
                        asio::write(streetSocket, asio::buffer(carJSON));

                        
                        cout << "ABOUT TO READ" << endl;
                        string save = _read(socket);
                        cout << "HIER!!!!!! : " + save << endl;
                        */
                       cout << "HALLLLLLLLLLLLLLLLLO" << endl;
                    }
                    else
                    {
                        // t1 north to t1 south
                        nextCar.direction = NORTH;
                        string carJSON = getJSON(nextCar).dump() + "\n";
                        //send_stream << carJSON;
                        spdlog::get("console")->info("[T1 North] Sending to Server [T1 South] JSON: " + carJSON);

                        this_thread::sleep_for(chrono::milliseconds(1000));
                        asio::write(socket, asio::buffer(carJSON));
                    }

                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "NORTH", style::reset);
                    carQueue->pop();
                }
                else
                {
                    // traffic light t1 south

                    if (port == 47500)
                    {
                        /*
                        if (nextCar.direction == NORTH)
                        {
                            cout << "SHOULD SEND CAR INTO NOWEHERE" << endl;
                        }
                        else
                        {

                            cout << "About to send car to other street" << endl;
                            
                        asio::io_context ioCtx;

                        string host = "127.0.0.1";
                        string port = to_string(receiverPort);

                        asio::ip::tcp::resolver resolver(ioCtx.get_executor());
                        asio::ip::basic_resolver_results endpoint = resolver.resolve(host, port);

                        asio::ip::tcp::socket socket(ioCtx);
                        asio::connect(socket, endpoint);
                       


                        asio::streambuf sender;
                        ostream send_stream(&sender); */

                            //sending car item

                            /*

                            Car tempCar = carQueue->front();
                            string carJSON = getJSON(tempCar).dump();
                            //send_stream << carJSON;
                            cout << "JSON: " + carJSON << endl;

                            this_thread::sleep_for(chrono::milliseconds(5000));
                            //asio::write(socket, asio::buffer(carJSON));

                            cout << "ABOUT TO READ" << endl;
                            string save = _read(socket);
                            cout << "HIER!!!!!! : " + save << endl;

                            carQueue->pop();

                            cout << "Sent car to other street" << endl;

                            //write(socket, sender);
                            
                        }*/
                        cout << "OLLLLLLLLLLLLLLLLLAH" << endl;
                    }
                    else
                    {
                        // t2 south to t2 north
                        nextCar.direction = NORTH;
                        string carJSON = getJSON(nextCar).dump() + "\n";
                        //send_stream << carJSON;
                        spdlog::get("console")->info("[T2 South] Sending to Server [T2 North] JSON: " + carJSON);

                        this_thread::sleep_for(chrono::milliseconds(1000));
                        asio::write(tmpsocket, asio::buffer(carJSON));
                    }
                    println("[CAR] ", fg::cyan, nextCar.getName(), " ", nextCar.getLicensePlate(), " drives away from ", style::bold, "SOUTH", style::reset);
                    carQueue->pop();

                    //socket.shutdown(asio::ip::tcp::socket::shutdown_both);
                    //socket.close();
                }
                this_thread::sleep_for(chrono::milliseconds(nextCar.getSpeed()));

                /*
            } catch(exception& e) {
                println(e.what());
            }
                this_thread::sleep_for(chrono::milliseconds(5000));
                if (++counter > 3)
                {
                    streetSocket.close();
                    exit(0);
                }*/
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
