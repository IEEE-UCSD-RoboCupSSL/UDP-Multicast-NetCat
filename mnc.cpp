#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>

#define DURATION_MILLIS_COUNT(time_point) std::chrono::duration_cast<std::chrono::milliseconds> (time_point).count()


using namespace boost;
using namespace boost::asio;


bool isServerMode = false;
int port = -1;
std::string ipAddr = "";


void printHelp() {
    std::cout << "usage: mnc ([-l listen/server mode]) [destination multicast ip address] [port]" << std::endl;
}

bool processArgs(int argc, char *argv[]) {

    if(argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        printHelp();
        return false;
    }

    char option;
    while ( (option = getopt(argc, argv,":l")) != -1 ) {
        switch(option) {
            case 'l':
                isServerMode = true;
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                printHelp();
                return false;
                break;
        } 
    }

    ipAddr = std::string(argv[argc - 2]);
    port =  std::atoi(argv[argc - 1]);

    if(port == -1) {
        std::cout << "missing port" << std::endl;
        printHelp();
        return false;
    }
    if(ipAddr == "localhost") ipAddr = "127.0.0.1";
    if(ipAddr == "") {
        std::cout << "missing destination ip addr" << std::endl;
        printHelp();
        return false;
    }
    return true;
}


int main(int argc, char *argv[]) {
    
    const auto period = std::chrono::milliseconds(10);
    auto t0 = std::chrono::steady_clock::now();

    if(!processArgs(argc, argv)) return -1;

    io_service ios;
    ip::udp::endpoint ep(ip::address::from_string(ipAddr), port);
    
    auto receive_buffer = boost::array<char, 1024>();
    auto socket = ip::udp::socket(ios);

    socket.open(ep.protocol());
    socket.set_option(ip::udp::socket::reuse_address(true));
    socket.bind(ep);
    socket.set_option(ip::multicast::join_group(ep.address()));

    
    size_t packetSize;
    std::string packetString;

    while(true) {
        auto t = std::chrono::steady_clock::now(); 

//        std::cout << DURATION_MILLIS_COUNT(std::chrono::steady_clock::now() - t0) << std::endl;
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if(isServerMode) {
            packetSize = socket.receive_from(asio::buffer(receive_buffer), ep);
            packetString = std::string(receive_buffer.begin(), receive_buffer.begin() + packetSize);
            std::cout << packetString << std::flush;
        } else {
            std::getline(std::cin, packetString);
            socket.send_to(asio::buffer(packetString + "\n"), ep);
        }


        std::this_thread::sleep_until(t + period);
    }


   
    return 0;
}

