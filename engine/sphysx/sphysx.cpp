#include <sphysx/sphysx.h>
namespace engine::sphysx
{
    physx_base::physx_base()
    {
    }

    physx_base::~physx_base()
    {
    }
    Snippets::Camera* sCamera;
    void motionCallback(int x, int y){
        sCamera->handleMotion(x, y);
    }
    void keyboardCallback(unsigned char key, int x, int y){
        if (key == 27)
			exit(0);

		if (!sCamera->handleKey(key, x, y))
			keyPress(key, sCamera->getTransform());
    }
    void mouseCallback(int button, int state, int x, int y){
        sCamera->handleMouse(button, state, x, y);
    }
    void idleCallback(){
        glutPostRedisplay();
    }
    void renderCallback(){
        stepPhysics(true);

		Snippets::startRender(sCamera->getEye(), sCamera->getDir());

		physx::PxScene* scene;
		PxGetPhysics().getScenes(&scene, 1);
		physx::PxU32 nbActors = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors)
		{
			std::vector<physx::PxRigidActor*> actors(nbActors);
			scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<physx::PxActor**>(&actors[0]), nbActors);
			Snippets::renderActors(&actors[0], static_cast<physx::PxU32>(actors.size()), true);
		}

		Snippets::finishRender();
    }
    void exitCallback(void){
        delete sCamera;
		cleanupPhysics(true);
    }

    void physx_base::renderLoop()
    {
        sCamera = new Snippets::Camera(physx::PxVec3(50.0f, 50.0f, 50.0f), physx::PxVec3(-0.6f, -0.2f, -0.7f));

        Snippets::setupDefaultWindow("PhysX Snippet HelloWorld");
        Snippets::setupDefaultRenderState();

        glutIdleFunc(idleCallback);
        glutDisplayFunc(renderCallback);
        glutKeyboardFunc(keyboardCallback);
        glutMouseFunc(mouseCallback);
        glutMotionFunc(motionCallback);
        motionCallback(0, 0);

        atexit(exitCallback);

        initPhysics(true);
        glutMainLoop();
    }

}
