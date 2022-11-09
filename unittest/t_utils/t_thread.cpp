#include <string>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
class mtx
{
public:
	void lock()
	{
	}
	void unlock()
	{
	}
};
class t_thread
{
	int index = 0;
	mtx mtx_;
	std::mutex mutex_;
	int get_index()
	{
		std::lock_guard<mtx> lock(mtx_);
		//std::lock_guard<std::mutex> lock(mutex_);
		std::this_thread::sleep_for(std::chrono::microseconds(10));
		std::cout << std::this_thread::get_id() << " = " << index << std::endl;
		return index++;
	}
	std::vector<std::thread> t_vec_;
public:
	virtual ~t_thread() {}
	virtual void excute()
	{
		for (int i = 0; i < 2; ++i)
		{
			t_vec_.emplace_back(std::move(std::thread([i, this]() {
				if (i == 1)
				{

				}
				while (true)
				{
					get_index();
					std::this_thread::sleep_for(std::chrono::microseconds(10));
				}
				})));
		}
		std::cout << std::this_thread::get_id() << std::endl;
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	t_thread* sln = new t_thread();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
