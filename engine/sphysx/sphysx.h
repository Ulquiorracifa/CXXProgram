#pragma once
#include <vector>
#include <memory>
#include "PxPhysicsAPI.h"

#include <ctype.h>
#include "SnippetPrint.h"
#include "SnippetPVD.h"
#include "SnippetUtils.h"

#include "SnippetRender.h"
#include "SnippetCamera.h"
#include <utils/platform.hpp>
using namespace physx;
namespace engine::sphysx
{
    class EngineAPI physx_base :std::enable_shared_from_this<physx_base>
    {
        public:
            physx_base();
            virtual ~physx_base();
            int execute();
    };
}
