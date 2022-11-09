#include <cstdlib>
#include <iostream>
#include <streambuf>
#include <memory>
#include <utility>
#include <thread>
#include <chrono>
#include <time.h> 
#include <string>
#include <string.h>
#include <asio.hpp>
#include <logger/logger.h>
#include <spdlog/spdlog.h>
#include <utils/tools.hpp>

class t_asio_context
{
public:
    t_asio_context() {}
    virtual ~t_asio_context()
    {
        if (thread_.joinable()) 
        {
            child_context_.stop();
            thread_.join();
        }
    }

    void main_child_excute()
    {
        std::cout << "main thread_id = " << std::this_thread::get_id() << std::endl;
        thread_ = std::thread([this]()
            {
                auto work = asio::make_work_guard(child_context_);
                child_context_.run();
            });
        
  
        
        for (int i = 0; i < 20; ++i)
        {
          /*  child_context_.dispatch(std::bind([=]() {
                std::cout << "child dispatch start  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                main_context_.dispatch(std::bind([=]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    num++;
                    std::cout << "main dispatch in  thread_id = " << std::this_thread::get_id() << " i :" << i  << "  , num = " << num << std::endl;
                    }));
                std::cout << "child dispatch end  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                }));*/
            
                asio::post(child_context_,std::bind([=]() {
                    std::cout << "child post start  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                    for (int y = 0; y < 10240; ++y) 
                    {
                        main_context_.post(std::bind([=]() {
                            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            num++;
                            std::cout << "main dispatch in  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                            }));
                    }
                  
                    std::cout << "child post end  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                }));
        }
        auto work = asio::make_work_guard(main_context_);
        int32_t interval = 20;
        auto last_time = engine::utils::get_mill_time()+ interval;
        while (true)
        {
            while (main_context_.poll_one())
            {
                auto now = engine::utils::get_mill_time();
                if (now > last_time) 
                {
                    last_time = now + interval;
                    break;
                }
            }
            std::cout << "--------------------------- split line ------------------------" << std::endl;
            std::cout << "--------------------------- split line ------------------------" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void threads_excute()
    {
        int i = 0;
        for (i = 0; i < 4; ++i)
        {
            child_contexts_.emplace_back(new asio::io_context());
        }
        for(i =0;i<4;++i)
        {
           
            threads_.emplace_back(std::thread([i,this]() {
                auto work = asio::make_work_guard(child_contexts_[i]);
                child_contexts_[i]->run();
            }));
        }
        for (i = 0; i < 4; ++i)
        {
            auto& ctx = *child_contexts_[i];
            asio::post(ctx, std::bind([=]() {
                std::cout << "child post start  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                for (int y = 0; y < 10240; ++y)
                {
                    main_context_.post(std::bind([=]() {
                        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        num++;
                        std::cout << "main dispatch in  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                        }));
                }

                std::cout << "child post end  thread_id = " << std::this_thread::get_id() << " i :" << i << "  , num = " << num << std::endl;
                }));
        }
        
        auto work = asio::make_work_guard(main_context_);
        int32_t interval = 20;
        auto last_time = engine::utils::get_mill_time() + interval;
        while (true)
        {
            while (main_context_.poll_one())
            {
                auto now = engine::utils::get_mill_time();
                if (now > last_time)
                {
                    last_time = now + interval;
                    break;
                }
            }
            std::cout << "--------------------------- split line ------------------------" << std::endl;
            std::cout << "--------------------------- split line ------------------------" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    virtual void excute()
    {
       
        //std::streambuf stl_buff();
       
        engine::logger::default_init("", "asio", true,false, 1, "[%Y-%m-%d %H:%M:%S %z] [%n] [%^%l%$]%v");
        main_child_excute();
        //threads_excute();
        
    }
private:
    std::mutex mutex_;
    volatile int num = 0;
    asio::io_context child_context_;
    asio::io_context main_context_;
    std::thread thread_;
    std::thread post_thread_;
    std::vector<std::thread> threads_;
    std::vector<asio::io_context*> child_contexts_;
};


int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    t_asio_context* sln = new t_asio_context();
    sln->excute();
    delete sln;
    printf("------ end ------\n");
    return 0;
}
