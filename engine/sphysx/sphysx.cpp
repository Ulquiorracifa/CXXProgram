#include <sphysx/sphysx.h>
extern int snippetMain();
namespace engine::sphysx
{
    physx_base::physx_base()
    {
    }

    physx_base::~physx_base()
    {
    }
    
    int physx_base::execute()
    {
        return snippetMain();
    }
}