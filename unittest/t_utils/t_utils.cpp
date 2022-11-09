#include <string>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <utils/snowflake.hpp>
class t_utils
{
public:
	virtual ~t_utils() {}
	virtual void excute() 
	{
		engine::utils::snowflake<> uuid;
		uuid.init(1, 1);

		for (int64_t i = 0; i < 10000; ++i)
		{
			auto id = uuid.next();
			std::cout << id << "\n";
		}
	}
};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	t_utils* sln = new t_utils();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
