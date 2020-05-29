#ifndef HHLOG_H
#define HHLOG_H

#include<string>
#include <vector>

namespace hhit
{
    void log_init(char** argv);
    
    void log_buf(unsigned char* buff, size_t size, std::string tag = "");

    void log_hex(unsigned char c, std::string tag = "");
    
    void log_buf(std::vector<unsigned char> buf, std::string tag = "");
}
#endif
