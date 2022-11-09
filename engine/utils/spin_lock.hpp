#pragma once
#include <atomic>
#include <thread>
#include <stdint.h>
#include <utils/platform.hpp>
class EngineAPI spin_lock
{
	std::atomic_flag atm_flag_ = ATOMIC_FLAG_INIT;
public:
	spin_lock()
	{
	}

	~spin_lock()
	{
	}

	void lock()
	{
		for (uint64_t k = 0; try_lock(); ++k)
		{
			if (k % 1024 == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		return;
	}

	bool try_lock_times(uint64_t times)
	{
		for (uint64_t k = 0; try_lock(); ++k)
		{
			if (k >= times)
			{
				return false;
			}
		}
		return true;
	}

	bool try_lock()
	{
		bool ret = atm_flag_.test_and_set(/*std::memory_order_acquire*/);
		return ret;
	}

	void unlock()
	{
		atm_flag_.clear(/*std::memory_order_release*/);
		return;
	}
};
