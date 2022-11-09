#pragma once
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <memory>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
namespace engine::utils 
{
    class snowflake_nonlock
    {
    public:
        void lock()
        {
        }
        void unlock()
        {
        }
    };
    /**
     * 0 - 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000
     * 1位标识，占用1bit，其值始终是0，没有实际作用。 
     * 41位时间截(毫秒级)，注意，41位时间截不是存储当前时间的时间截，而是存储时间截的差值（当前时间截 - 开始时间截)
     * 得到的值），这里的的开始时间截，一般是我们的id生成器开始使用的时间，由我们程序来指定的（如下下面程序IdWorker类的startTime属性）。41位的时间截，可以使用69年，年T = (1L << 41) / (1000L * 60 * 60 * 24 * 365) = 69
     * 10位的数据机器位，可以部署在1024个节点，包括5位datacenterId和5位workerId
     * 12位序列，毫秒内的计数，12位的计数顺序号支持每个节点每毫秒(同一机器，同一时间截)，0开始不断累加，最多可以累加到4095，一共可以产生4096个ID   
     * 加起来刚好64位，为一个Long型。
     * SnowFlake的优点是，整体上按照时间自增排序，并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分)，并且效率较高，经测试，SnowFlake每秒能够产生26万ID左右。
 */
    template<typename Lock = snowflake_nonlock,int64_t twepoch = 1534780800000L /*2018-08-21*/>
    class EngineAPI snowflake : private noncopyable
    {
        using lock_type = Lock;
        static constexpr int64_t TWEPOCH = twepoch;
        static constexpr int64_t WORKER_ID_BITS = 5L;
        static constexpr int64_t DATACENTER_ID_BITS = 5L;
        static constexpr int64_t MAX_WORKER_ID = (1 << WORKER_ID_BITS) - 1;
        static constexpr int64_t MAX_DATACENTER_ID = (1 << DATACENTER_ID_BITS) - 1;
        static constexpr int64_t SEQUENCE_BITS = 12L;
        static constexpr int64_t WORKER_ID_SHIFT = SEQUENCE_BITS;
        static constexpr int64_t DATACENTER_ID_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS;
        static constexpr int64_t TIMESTAMP_LEFT_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS + DATACENTER_ID_BITS;
        static constexpr int64_t SEQUENCE_MASK = (1 << SEQUENCE_BITS) - 1;

        using time_point = std::chrono::time_point<std::chrono::steady_clock>;

        time_point start_time_point_ = std::chrono::steady_clock::now();
        int64_t start_millsecond_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        int64_t last_timestamp_ = -1;
        int64_t workerid_ = 0;
        int64_t datacenterid_ = 0;
        int64_t sequence_ = 0;
        lock_type lock_;
    public:
        snowflake() = default;

        snowflake(const snowflake&) = delete;

        snowflake& operator=(const snowflake&) = delete;

        void init(int64_t workerid, int64_t datacenterid)
        {
            if (workerid > MAX_WORKER_ID || workerid < 0) {
                throw std::runtime_error("worker Id can't be greater than 31 or less than 0");
            }

            if (datacenterid > MAX_DATACENTER_ID || datacenterid < 0) {
                throw std::runtime_error("datacenter Id can't be greater than 31 or less than 0");
            }

            workerid_ = workerid;
            datacenterid_ = datacenterid;
        }

        int64_t next()
        {
            std::lock_guard<lock_type> lock(lock_);
            //std::chrono::steady_clock  cannot decrease as physical time moves forward
            auto timestamp = millsecond();
            if (last_timestamp_ == timestamp)
            {
                sequence_ = (sequence_ + 1) & SEQUENCE_MASK;
                if (sequence_ == 0)
                {
                    timestamp = wait_next_millis(last_timestamp_);
                }
            }
            else
            {
                sequence_ = 0;
            }

            last_timestamp_ = timestamp;

            return ((timestamp - TWEPOCH) << TIMESTAMP_LEFT_SHIFT)| (datacenterid_ << DATACENTER_ID_SHIFT)| (workerid_ << WORKER_ID_SHIFT)| sequence_;
        }

    private:
        int64_t millsecond() const noexcept
        {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time_point_);
            return start_millsecond_ + diff.count();
        }

        int64_t wait_next_millis(int64_t last) const noexcept
        {
            auto timestamp = millsecond();
            while (timestamp <= last)
            {
                timestamp = millsecond();
            }
            return timestamp;
        }
    };
}

using snowflake_st = engine::utils::snowflake<>;
using snowflake_ptr_st = std::shared_ptr<snowflake_st>;
using snowflake_mt = engine::utils::snowflake<std::mutex>;
using snowflake_ptr_mt = std::shared_ptr < snowflake_mt>;

/***
*   单线程
*   using snowflake_t = snowflake<>;
*   snowflake_t uuid;
*   uuid.init(1, 1);
*   
*   for (int64_t i = 0; i < 10000; ++i)
*   {
*       auto id = uuid.next();
*       std::cout << id << "\n";
*   }
*/


/*
* 线程安全
* using snowflake_t = snowflake<std::mutex,1534832906275L>;
* snowflake_t uuid;
* uuid.init(1, 1);
*
* for (int64_t i = 0; i < 10000; ++i)
* {
*     auto id = uuid.next();
*     std::cout << id << "\n";
* }
*/
