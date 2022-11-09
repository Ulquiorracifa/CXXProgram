#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <thread>
#include <chrono>
#include <time.h> 
#include <string>
#include <asio.hpp>
#include <logger/logger.h>
#include <spdlog/spdlog.h>
#include <utils/noncopyable.hpp>
#include <utils/tools.hpp>
#include <net/server_base.h>
#include <net/decl.h>
#include <net/config.h>



class t_asio_client
{
    //static const int robot_count = 500;
    static const int64_t interval  = 10*1000;
    int64_t count = 0;
    std::shared_ptr<server_base> server_;

public:    
    virtual ~t_asio_client() {}
	virtual void excute() 
	{
        //std::string ip = "10.246.40.20";
        std::cout << "please enter remote ip :";
        std::string ip = "";
        std::cin >> ip;
        if (ip.empty())
        {
            //ip = "10.246.40.20";
            ip = "10.246.60.251";
        }
        std::cout << " remote ip is " << ip << std::endl;

        std::cout << "please enter remote port :";
        int port = 8889;
        std::cin >> port;
        if (port < 8889)
        {
            return;
        }
        std::cout << " remote port is " << port << std::endl;

        std::cout << "please enter robot count :";
        int robot_count = 0;
        std::cin >> robot_count;
        if (robot_count == 0)
        {
            return;
        }
        std::cout << "robot count is :"<< robot_count << std::endl;
       
        engine::logger::default_init("", "asio_client", true, false, 1, "[%Y-%m-%d %H:%M:%S %z] [%n] [%^%l%$]%v");

        net_options opts;
        opts.threads = 0;
        server_ = std::make_shared<server_base>(opts, [this](const net_packet_ptr& net_pkg)->bool {
            if (net_pkg->msgid == 1) 
            {
                this->server_->send(net_pkg->session, 1000, nullptr, 0);
            }
            else if(net_pkg->msgid == 1001)
            {
                count++;
                this->server_->send(net_pkg->session, 1000, nullptr, 0);
            }
            return true;
            });

      
        for (int i = 0; i < robot_count; ++i)
        {
            server_->connect(ip, port, socket_type::tcp);
        }
        last_time_ = engine::utils::get_mill_time()+ interval;
        while (true)
        {
            int64_t now = engine::utils::get_mill_time();
            if (now > last_time_)
            {
                std::cout << "qps = " << int(count * 1000 / interval) << std::endl;
                last_time_ = now + interval;
                count = 0;
            }
            server_->update(now);
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
	}
    int64_t last_time_;
};

int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    t_asio_client* sln = new t_asio_client();
    sln->excute();
    delete sln;
    printf("------ end ------\n");
    return 0;
}

