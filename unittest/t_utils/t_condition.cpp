#include <string>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <condition_variable>
#include <utils/noncopyable.hpp>
template <typename T>
class MessageQueue : noncopyable {
public:
    typedef T ValueType;
    typedef std::vector<ValueType> ContainerType;
    typedef std::mutex Mutex;
    typedef std::condition_variable Condition;
    typedef std::unique_lock<Mutex> LockGuard;

    void Push(ValueType&& v) {
        LockGuard guard(mutex_);
        queue_.emplace_back(std::move(v));
        guard.unlock();
        if (Notify) cond_.notify_one();
    }

    void Push(const ValueType& v) {
        LockGuard guard(mutex_);
        queue_.push_back(v);
        guard.unlock();
        if (Notify) cond_.notify_one();
    }

    bool Pop(ContainerType& v) {
        LockGuard guard(mutex_);
        if (Notify) {
            while (queue_.empty()) {
                cond_.wait(guard);
            }
        }
        std::swap(queue_, v);
        queue_.clear();
        return true;
    }

    uint64_t Size() const {
        LockGuard guard(mutex_);
        return queue_.size();
    }

    void disable_notify() {
        this->Notify = false;
    }

private:
    ContainerType queue_;
    Mutex mutex_;
    Condition cond_;

    bool Notify = true;
};

class t_condition
{
    void mut_thread() 
    {
        std::vector<std::thread> threads_;
        MessageQueue<std::string> queue_;
        for (int i = 0; i < 4; ++i)
        {
            threads_.emplace_back(std::thread([&]() {
                int num = 0;
                while (true)
                {
                    num++;
                    auto out = std::to_string(i) + ":" + std::to_string(num);
                    queue_.Push(out);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                }));
        }

        while (true)
        {
            std::vector<std::string> out_vec;
            queue_.Pop(out_vec);
            for (auto iter = out_vec.begin(); iter != out_vec.end(); ++iter)
            {
                std::cout << *iter << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::thread thread_;
    std::mutex mtx_;
    std::condition_variable cond_;
    std::unique_lock<std::mutex> lock_guard_;
    void sig_thread()
    {
        thread_ = std::thread([this]()
            {
                while (true)
                {
                   // cond_.notify_one();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            });
        int count = 1;
        while (true)
        {
            std::cout << "------------ count in "  << count << "--------------" << std::endl;
            std::unique_lock<std::mutex> guard(mtx_);
            cond_.wait(guard);
            std::cout << "------------ count ou " << count << "--------------" << std::endl;
            count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
public:
    virtual ~t_condition() {}
	virtual void excute()
	{
        //sig_thread();
	}

  
};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
    t_condition* sln = new t_condition();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
