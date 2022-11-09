#pragma once
#include <vector>
#include <memory>
#include "PxPhysicsAPI.h"

#include "SnippetRender.h"
#include "SnippetCamera.h"
#include <utils/platform.hpp>
extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive);
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const physx::PxTransform& camera);
namespace engine::sphysx
{
    class EngineAPI physx_base :std::enable_shared_from_this<physx_base>
    {
        public:
            physx_base();
            virtual ~physx_base();
            void renderLoop();
        private:
            Snippets::Camera* sCamera;
            physx::PxDefaultAllocator		gAllocator;
            physx::PxDefaultErrorCallback	gErrorCallback;

            physx::PxFoundation* gFoundation = NULL;
            physx::PxPhysics* gPhysics = NULL;

            physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
            physx::PxScene* gScene = NULL;

            physx::PxMaterial* gMaterial = NULL;

            physx::PxPvd* gPvd = NULL;

            physx::PxReal stackZ = 10.0f;
    };
}
