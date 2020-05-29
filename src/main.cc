//
//  main.cpp
//  isserver
//
//  Created by 王晓鹏 on 2020/5/27.
//  Copyright © 2020 王晓鹏. All rights reserved.
//

#include <iostream>
#include "handy.h"
#include "spdlog.h"
#include "sinks/basic_file_sink.h"
#include "hhlog.h"
#include <glog/logging.h>
#include "timer.h"

using namespace std;
using namespace handy;
using namespace hhit;


void printBuffer(char* buff, int size, std::string tag){
    std::cout << tag << std::endl;
    for(int i = 0; i < size; i++){
        // printf("%02x", buff[i]);
        printf("%02x", buff[i]);
    }
    std::cout << std::endl << std::endl;
}



int main(int argc, char** argv) {
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
    
    
    
    
    log_init(argv);
    
    Ip4Addr peer_;
    
    
    setloglevel("INFO");
    EventBase base;
    Signal::signal(SIGINT, [&] { base.exit(); });
    UdpServerPtr svr = UdpServer::startServer(&base, "", 2099);
    exitif(!svr, "start udp server failed");
    svr->onMsg([&](const UdpServerPtr &p, Buffer buf, Ip4Addr peer) {
        
        log_buf((unsigned char*)buf.data(), buf.size(), "");
        peer_ = peer;
   
        Buffer send_buf;
        
        char type = buf.data()[7];
       
        if(type == 0x01) {
            char id[] = {0x68,
                        0x68, 0x68, 0x69, 0x74, 0x00, 0x00,
                        0x01,
                        0x06, 0x00,
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                        0x00 ,0x16};

            send_buf.append(id, sizeof(id));
            
            log_buf((unsigned char*)send_buf.data(), send_buf.size(), "校时命令");
            svr->sendTo(send_buf, peer);

            
            
            Timer timer_;
            timer_.setTimeout([&](void){
                Buffer psd_buf;
                char psd_data[] = {0x68,
                    0x68, 0x68, 0x69, 0x74, 0x00, 0x00,
                    0x02,
                    0x08, 0x00,
                    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                    0x00 ,0x16};
                
                psd_buf.append(psd_data, sizeof(psd_data));
                
                log_buf((unsigned char*)psd_buf.data(), psd_buf.size(), "设置终端密码");
                svr->sendTo(psd_buf, peer_);
                
                
                Buffer config_buf;
                char config_data[] = {0x68,
                    0x68, 0x68, 0x69, 0x74, 0x00, 0x00,
                    0x03,
                    0x12, 0x00,
                    0x35, 0x36, 0x37, 0x38, 0x01, 0x14, 0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x00, 0x08, 0x00,
                    0x00 ,0x16};
                
                config_buf.append(config_data, sizeof(config_data));
                
                log_buf((unsigned char*)config_buf.data(), config_buf.size(), "主站下发参数配置");
                svr->sendTo(config_buf, peer_);
                
                
                
                
            }, 2000);
        }else {
            send_buf.append(buf.data(), buf.size());
            p->sendTo(send_buf, peer);
        }
        
        
        // hinfo("echo msg: %s to %s", buf.data(), peer.toString().c_str());
    });
    

    
    
    base.loop();
    
    

}
