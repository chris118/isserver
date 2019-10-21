#include "threadpool.h"
#include <iostream>

#include "handy.h"
#include "spdlog.h"
#include "sinks/basic_file_sink.h"
#include "sinks/daily_file_sink.h"

using namespace handy;


int fun1(int slp)
{
    // printf("  hello, fun1 !  %d\n" ,std::this_thread::get_id());
    if (slp>0) {
        printf(" ======= fun1 sleep %d  =========  %d\n",slp, std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(slp));
        return 88;
    }
}

struct gfun {
    int operator()(int n) {
        printf("%d  hello, gfun !  %d\n" ,n, std::this_thread::get_id() );
        return 42;
    }
};

class A { 
public:
    static int Afun(int n = 0) {   //函数必须是 static 的才能直接使用线程池
        std::cout << n << "  hello, Afun !  " << std::this_thread::get_id() << std::endl;
        return n;
    }

    static std::string Bfun(int n, std::string str, char c) {
        std::cout << n << "  hello, Bfun !  "
                       << str.c_str()
                       <<"  " << (int)c
                       <<"  " << std::this_thread::get_id() 
                       << std::endl;
        return str;
    }
};


int main(int argc, const char *argv[]) {

    ////////////////////////////// 
    //
    //  test spdlog
    //
    /////////////////////////////

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
    
    // // Set the default logger to file logger
    // auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
    // spdlog::set_default_logger(file_logger);     

     // Create a daily logger - a new file is created every day on 2:30am
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30); 
    // spdlog::set_default_logger(daily_logger);     

    ////////////////////////////// 
    //
    //  test handy
    //
    /////////////////////////////

    setloglevel("TRACE");
    EventBase base;
    Signal::signal(SIGINT, [&] { base.exit(); });
    UdpServerPtr svr = UdpServer::startServer(&base, "", 2099);
    exitif(!svr, "start udp server failed");
    svr->onMsg([](const UdpServerPtr &p, Buffer buf, Ip4Addr peer) {
        hinfo("echo msg: %s to %s", buf.data(), peer.toString().c_str());
        p->sendTo(buf, peer);
    });
    base.loop();

      
    ////////////////////////////// 
    //
    //  test thread pool
    //
    /////////////////////////////
    //  try {
    //     // hhit::threadpool executor{ 50 };


    //     // std::future<int> ff = executor.commit(fun1,1000);
    //     // std::cout << ff.get() <<  "  " << std::this_thread::get_id() << std::endl;
    //     // std::cout   << " 111 " << std::endl;

    //     // for (int i = 0; i < 50; i++) {
    //     //     executor.commit(fun1,i );
    //     // }


    //     hhit::threadpool pool(4);
    //     std::vector< std::future<int> > results;

    //     for (int i = 0; i < 8; ++i) {
    //         results.emplace_back(
    //             pool.commit([i] {
    //                 std::cout << "hello " << i << std::endl;
    //                 std::this_thread::sleep_for(std::chrono::seconds(1));
    //                 std::cout << "world " << i << std::endl;
    //                 return i*i;
    //             })
    //         );
    //     }
    //     std::cout << " =======  commit all2 ========= " << std::this_thread::get_id() << std::endl;

    //     for (auto && result : results)
    //         std::cout << "result: " << result.get() << ' '<< std::endl;
    //     std::cout << "======= end =======" << std::endl;


    //     // A a;
    //     // std::future<void> ff = executor.commit(fun1,0);
    //     // std::future<int> fg = executor.commit(gfun{},0);
    //     // std::future<int> gg = executor.commit(a.Afun, 9999); //IDE提示错误,但可以编译运行
    //     // std::future<std::string> gh = executor.commit(A::Bfun, 9998,"mult args", 123);
    //     // std::future<std::string> fh = executor.commit([]()->std::string { std::cout << "hello, fh !  " << std::this_thread::get_id() << std::endl; return "hello,fh ret !"; });

    //     // std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
    //     // std::this_thread::sleep_for(std::chrono::microseconds(900));

    //     // for (int i = 0; i < 50; i++) {
    //     //     executor.commit(fun1,i*100 );
    //     // }
    //     // std::cout << " =======  commit all ========= " << std::this_thread::get_id()<< " idlsize="<<executor.idlCount() << std::endl;

    //     // std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
    //     // std::this_thread::sleep_for(std::chrono::seconds(3));

    //     // ff.get(); //调用.get()获取返回值会等待线程执行完,获取返回值
    //     // std::cout << fg.get() << "  " << fh.get().c_str()<< "  " << std::this_thread::get_id() << std::endl;

    //     // std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
    //     // std::this_thread::sleep_for(std::chrono::seconds(3));

    //     // std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
    //     // executor.commit(fun1,55).get();    //调用.get()获取返回值会等待线程执行完

    //     // std::cout << "end... " << std::this_thread::get_id() << std::endl;


    //     // hhit::threadpool pool(4);
    //     // std::vector< std::future<int> > results;

    //     // for (int i = 0; i < 8; ++i) {
    //     //     results.emplace_back(
    //     //         pool.commit([i] {
    //     //             std::cout << "hello " << i << std::endl;
    //     //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //     //             std::cout << "world " << i << std::endl;
    //     //             return i*i;
    //     //         })
    //     //     );
    //     // }
    //     // std::cout << " =======  commit all2 ========= " << std::this_thread::get_id() << std::endl;

    //     // for (auto && result : results)
    //     //     std::cout << result.get() << ' ';
    //     // std::cout << std::endl;

    //     while (true)
    //     {
    //          std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //     }
        
    //     return 0;
    // }
    // catch (std::exception& e) {
    // std::cout << "some unhappy happened...  " << std::this_thread::get_id() << e.what() << std::endl;
}
