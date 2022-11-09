#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <asio.hpp>
#include <logger/logger.h>
#include <utils/tools.hpp>
#include <net/server_base.cpp>
#include <net/decl.h>
#include <net/config.h>
class t_asio_server
{
public:
    virtual ~t_asio_server() {}
    int64_t count = 0;
    static const int64_t interval = 10 * 1000;
    std::shared_ptr<server_base> server_;
	virtual void excute() 
	{
        std::cout << "please enter listener port :";
        int port = 8889;
        std::cin >> port;
        if (port < 8889)
        {
            return;
        }
        std::cout << " listener port is " << port << std::endl;

        std::cout << "please enter thread_count count :";
        int thread_count = 0;
        std::cin >> thread_count;
        if (thread_count < 0)
        {
            return;
        }
        std::cout << " thread_count count is " << thread_count << std::endl;
        engine::logger::default_init("", "asio", true, false, 1, "[%Y-%m-%d %H:%M:%S %z] [%n] [%^%l%$]%v");
        /*try
        {*/
            net_options opts;
            opts.threads = thread_count;
            server_ = std::make_shared<server_base>(opts,[this](const net_packet_ptr& net_pkg)->bool{
                count++;
                this->server_->send(net_pkg->session, 1001, nullptr, 0);
                return true;
                });
            server_->listen("0.0.0.0", port, socket_type::tcp);
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
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        /*}
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }*/
	}
    int64_t last_time_;
};


int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    srand((unsigned)time(nullptr));
    t_asio_server* sln = new t_asio_server();
    sln->excute();
    delete sln;
    printf("------ end ------\n");
    return 0;
}

