#include <memory>
#include <logger/logger.h>

class t_spdlog
{
public:
    virtual ~t_spdlog() {}
	virtual void excute() 
	{
        engine_logger_test();
	}


    void engine_logger_test()
    {
        engine::logger::default_init("logs/", "test", true, false, 1, "[%Y-%m-%d %H:%M:%S %z] [%n] [%^%l%$]%v");
        Log_Trace("async debug!");
        std::string  ip = "127.0.0.1";
        int port = 6379;
        //char* errstr = "connect failed";
        Log_Error("dbredisconnect::connect redis connect failed! ip = {}, port = {}", ip, port);
    }

    void spdlog_normal_test()
    {
        spdlog::flush_on(spdlog::level::warn);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_every(std::chrono::seconds(5));
        spdlog::set_pattern("[%Y-%m-%d.%e %H:%M:%S %z] [%n] [%^%l%$]%v");
        spdlog::trace("async trace!");
        spdlog::debug("async debug!");
        spdlog::info("async info!");
        spdlog::warn("async warn with args = {:08d}", 12);
        spdlog::error("async error with args = {}", "string");
        spdlog::critical("async critical with args = {}", 101);
    }

    void spdlog_sync_test()
    {
        auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto daily = std::make_shared < spdlog::sinks::daily_file_format_sink_mt>("logs/sync.log", 23, 59);

        std::vector < spdlog::sink_ptr > vec = { console ,daily };

        auto sync_logger = std::make_shared<spdlog::logger>("sync", vec.begin(), vec.end());
        sync_logger->flush_on(spdlog::level::warn);
        sync_logger->set_level(spdlog::level::debug);
        sync_logger->set_pattern("[%Y-%m-%d.%e %H:%M:%S %z] [%n] [%^%l%$]%v");
        spdlog::set_default_logger(sync_logger);
        spdlog::flush_every(std::chrono::seconds(5));

        sync_logger->info("sync info!");
        sync_logger->debug("sync debug with args = {}", 1.0f);
        sync_logger->error("sync error debug with args = {}", "string");
        sync_logger->warn("sync warn debug with args = {:08d}", 12);
    }

    void spdlog_async_test() 
    {
        auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto daily = std::make_shared < spdlog::sinks::daily_file_format_sink_mt>("logs/async.log",23,59);

        std::vector < spdlog::sink_ptr > vec = { console ,daily };

        auto thread_p = std::make_shared<spdlog::details::thread_pool>(8192, 1);
        auto async_logger = std::make_shared<spdlog::async_logger>("async", vec.begin(), vec.end(), thread_p,spdlog::async_overflow_policy::block);
        async_logger->flush_on(spdlog::level::warn);
        async_logger->set_level(spdlog::level::debug);
        async_logger->set_pattern("[%Y-%m-%d.%e %H:%M:%S %z] [%n] [%^%l%$]%v");
        
       //spdlog::register_logger(async_logger);
        spdlog::set_default_logger(async_logger);
        spdlog::flush_every(std::chrono::seconds(5));

        async_logger->info("async info!");
        async_logger->debug("async debug with args = {}", 1.0f);
        async_logger->error("async error debug with args = {}", "string");
        async_logger->warn("async warn debug with args = {:08d}", 12);
    }
};

int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    srand((unsigned)time(nullptr));
    t_spdlog* sln = new t_spdlog();
    sln->excute();
    delete sln;
    printf("------ end ------\n");
    return 0;
}

