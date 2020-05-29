#include <iostream>
#include "handy.h"
#include "spdlog.h"
#include "sinks/basic_file_sink.h"

using namespace std;
using namespace handy;


void printBuffer(char* buff, int size, std::string tag){
    std::cout << tag << std::endl;
        for(int i = 0; i < size; i++){
            // printf("%02x", buff[i]);
            printf("%02x", buff[i]);
    }
    std::cout << std::endl << std::endl;
}


int main(int argc, const char *argv[]) {
      spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");    
    
    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    
    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", {});
    SPDLOG_DEBUG("Some debug message");
    
    // Set the default logger to file logger
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
    spdlog::set_default_logger(file_logger);      






   setloglevel("TRACE");
   EventBase base;
    Signal::signal(SIGINT, [&] { base.exit(); });
    UdpServerPtr svr = UdpServer::startServer(&base, "", 2099);
    exitif(!svr, "start udp server failed");
    svr->onMsg([](const UdpServerPtr &p, Buffer buf, Ip4Addr peer) {
        printBuffer(buf.data(), buf.size());

        // hinfo("echo msg: %s to %s", buf.data(), peer.toString().c_str());
        p->sendTo(buf, peer);
    });
    base.loop();



      
}
