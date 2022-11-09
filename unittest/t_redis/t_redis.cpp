#include <string>
#include <redis/redis_connect.h>
#include <functional>

class t_redis
{
	void exc_func(int idx ,std::function<void(int)> func) 
	{
		func(idx);
	}
public:
	virtual ~t_redis(){}
	virtual void excute() 
	{
		exc_func(1001, [](int) {
				printf("index = %d \n",1001);
			});
		int i = 0;
		for (i = 0; i < 10; ++i) 
		{
			printf("1 -- %d \n", i);
			printf("2 -- %d \n", i);
		}


		std::string ip = "10.246.60.251";
		int32_t port = 6379;
		engine::redis::redis_connect connect(ip,port);
		connect.connect();
	}
};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	srand((unsigned)time(nullptr));
	t_redis* sln = new t_redis();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
