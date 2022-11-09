#include <string>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <utils/lockfree_queue.hpp>
#include <concurrentqueue/concurrentqueue.h>
class t_lockfree
{
public:
	
	virtual ~t_lockfree() {}

	void excute()
	{
		//atomic_test()
		//co_lock_free();
		producer_consumer();
	
	}

	/************************************/
	std::atomic<int> count = { 0 };
	std::vector<std::thread> t_vec_;
	void atomic_test()
	{
		for (int i = 0; i < 2; ++i)
		{
			t_vec_.emplace_back(std::move(std::thread([this]() {
				while (true)
				{
					//count.store(count,std::memory_order_relaxed);
					count++;

					int c = count.load(std::memory_order_acquire);
					std::cout << " count = " << c << std::endl;
					std::this_thread::sleep_for(std::chrono::microseconds(10));
				}
				})));
		}

		while (true)
		{
			//std::cout << " count = " << count << std::endl;
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	/*********************/

	moodycamel::ConcurrentQueue<int> q;
	int dequeued[100] = { 0 };
	std::thread threads[20];
	void co_lock_free()
	{
		// Producers
		for (int i = 0; i != 10; ++i) {
			threads[i] = std::thread([&](int i) {
				for (int j = 0; j != 10; ++j) {
					q.enqueue(i * 10 + j);
				}
				}, i);
		}

		// Consumers
		for (int i = 10; i != 20; ++i) {
			threads[i] = std::thread([&]() {
				/*int item;
				for (int j = 0; j != 20; ++j) {
					if (q.try_dequeue(item)) {
						++dequeued[item];
					}
				}*/
				});
		}

		// Wait for all threads
		for (int i = 0; i != 20; ++i) {
			threads[i].join();
		}

		// Collect any leftovers (could be some if e.g. consumers finish before producers)
		int item;
		while (q.try_dequeue(item)) {
			++dequeued[item];
		}
		while (q.try_dequeue(item)) {
			++dequeued[item];
		}

		// Make sure everything went in and came back out!
		for (int i = 0; i != 100; ++i) {
			assert(dequeued[i] == 1);
		}
	}

	struct Item
	{
		int id ;
	};

	moodycamel::ConcurrentQueue<Item> item_queue_;;
	static const int ProducerCount = 1;
	static const int ConsumerCount = 1;
	std::thread producers[ProducerCount];
	std::thread consumers[ConsumerCount];
	std::atomic<int> doneProducers = {0};
	std::atomic<int> doneConsumers = {0};
	std::atomic<size_t> index = { 0 };

	void producer_consumer()
	{
		for (int i = 0; i != ProducerCount; ++i) {
			producers[i] = std::thread([&]() {
				while (true) {
					Item item;
					index++;//.fetch_add(1, std::memory_order_release);
					item.id = index.load(std::memory_order_acquire);
					item_queue_.enqueue(item);
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
				});
		}
		for (int i = 0; i != ConsumerCount; ++i) {
			consumers[i] = std::thread([&]() {
				Item items[10];
				while (true) {
					std::size_t size = item_queue_.try_dequeue_bulk(items, 10);
					for (std::size_t c = 0; c < size; c++)
					{
							std::cout << "item_id = " << items[c].id << std::endl;
					}
					/*while (item_queue_.try_dequeue(item)) {
						std::cout << "item_id = " << item.id << std::endl;
					}*/
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
				});
		}
		for (int i = 0; i != ProducerCount; ++i) {
			producers[i].join();
		}
		for (int i = 0; i != ConsumerCount; ++i) {
			consumers[i].join();
		}
	}
private:
	engine::utils::Queue<int> queue_;
};


int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	t_lockfree* sln = new t_lockfree();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
