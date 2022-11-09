#include <string>
#include <sphysx/sphysx.h>

class t_physx
{
public:
	virtual ~t_physx(){}
	virtual void excute() 
	{
		engine::sphysx::physx_base *physxb = new engine::sphysx::physx_base();
		physxb->renderLoop();
	}
};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	srand((unsigned)time(nullptr));
	t_physx* sln = new t_physx();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
