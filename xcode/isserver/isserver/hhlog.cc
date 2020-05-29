#include "hhlog.h"

#include <iostream>
#include <thread>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <glog/logging.h>

using namespace std;

namespace hhit
{

    void log_init(char **argv) {
        //初始化参数
        FLAGS_logtostderr = false;  //TRUE:标准输出,FALSE:文件输出
        FLAGS_alsologtostderr = true;  //除了日志文件之外是否需要标准输出
        FLAGS_colorlogtostderr = true;  //标准输出带颜色
        FLAGS_logbufsecs = 0;   //设置可以缓冲日志的最大秒数，0指实时输出
        FLAGS_max_log_size = 50;  //日志文件大小(单位：MB)
        FLAGS_stop_logging_if_full_disk = true; //磁盘满时是否记录到磁盘
        google::InitGoogleLogging(argv[0]);
        std::string path = "./logs";
        mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
        google::SetLogDestination(google::GLOG_INFO,"./logs/");

    }
void log_buf(std::vector<unsigned char> buf, std::string tag) {
    if(buf.empty())
        return;
    log_buf(buf.data(), buf.size(), tag);
}
    
void log_buf(unsigned char*  buf, size_t size, std::string tag){
    google::LogMessage log(__FILE__, __LINE__);
    log.stream() << tag << "  " << "size: " << dec <<size  <<" type: " 
                << hex  << setfill('0') << setw(2) 
                << (unsigned int)(unsigned char)buf[7]<< endl;

    for (int i = 0; i < size; i++) {
		if (i == size - 1) {
			log.stream()  <<hex << setfill('0') << setw(2) << (unsigned int)(unsigned char)buf[i];
			log.stream() << endl;	
		}
		else if (i != 0 && i % 20 == 0) {
			log.stream() << std::endl;
			log.stream()  << hex << setfill('0') << setw(2) << (unsigned int)(unsigned char)buf[i] << ' ';
		}
		else {
			log.stream()  << hex << setfill('0') << setw(2) << (unsigned int)(unsigned char)buf[i] << ' ';
		}
	}
    log.stream() << endl;
	log.stream() << std::defaultfloat;
}

void log_hex(unsigned char c, std::string tag){
    LOG(INFO) << tag<< " " << std::setfill('0') << std::setw(3) << std::hex  << c;
}

} 



