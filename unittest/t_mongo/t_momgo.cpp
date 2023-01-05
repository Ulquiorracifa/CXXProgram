#include <string>
#include <mongo/mongo_client.h>

class t_mongo
{
public:
	virtual ~t_mongo(){}
	virtual void excute(int, const char*const*) 
	{
		engine::mongodb::mongo_client *mongo_client = new engine::mongodb::mongo_client("127.0.0.1", "27017");
		mongo_client->connectToHost();
	}
};

int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    srand((unsigned)time(nullptr));
    t_mongo* sln = new t_mongo();
    sln->excute(argc, argv);
    delete sln;
    printf("------ end ------\n");
    return 0;
}