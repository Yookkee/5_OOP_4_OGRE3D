/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
	toggleTimer = 0;
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
	if (mCameraMan) delete mCameraMan;
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)

	// Turn on the lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Ask the SceneManager to create an Entity
	Ogre::Entity* myEntity = mSceneMgr->createEntity("penguin.mesh");
	//ogreEntity->setMaterialName("test3.material");
	// Create shadows
	myEntity->setCastShadows(true);
	// We need to create a SceneNode so the Entity can be displayed in our scene
	Ogre::SceneNode* entityNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("entityNode");
	// Attach our Entity to it
	entityNode->attachObject(myEntity);
	entityNode->setPosition(0, 25, 0);

	// Create an abstract Plane object. 
	// This is not the mesh, it is more of a blueprint.
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	// Ask the MeshManager to create us a mesh using our Plane blueprint.
	// Basically, we've create a new mesh called "ground" with a size of 1500x1500.
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		1500, 1500, 20, 20, 
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z
	);
	// Now we will create a new Entity using this mesh.
	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	// tell our SceneManager not to cast shadows from our ground Entity.
	groundEntity->setCastShadows(false);
	// give our ground a material
	groundEntity->setMaterialName("Examples/Rockwall");

	//Ogre::Light* light = mSceneMgr->createLight("MainLight");
	//light->setPosition(20, 80, 50);

	// Let's add a Light to our scene.
	Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
	// We'll set the diffuse and specular colors to pure blue.
	spotLight->setDiffuseColour(0, 0.5, 1.0);
	spotLight->setSpecularColour(0, 0.5, 1.0);
	// Next we will set the type of the light to spotlight.
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	// The spotlight requires both a position and a direction - remember it acts like a flashlight
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(200, 200, 0));
	// Finally, we set what is called the spotlight range.
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	// add a directional light to our scene. 
	// This type of light essentially simulates daylight or moonlight.
	// The light is cast at the same angle across the entire scene equally.
	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	// Set Dark Red color
	directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
	// set the Light's direction. 
	// A directional light does not have a position 
	// because it is modeled as a point light that is infinitely far away.
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	// add a dark gray point light to our scene.
	Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setDiffuseColour(.3, .3, .3);
	pointLight->setSpecularColour(.3, .3, .3);
	//place our last light above and behind the entity
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));

}

//---------------------------------------------------------------------------
void TutorialApplication::createCamera()
{
	// Asking the SceneManager to create a new Camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	// Position the Camera and set it's direction
	mCamera->setPosition(Ogre::Vector3(0, 300, 500));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	// This is the distance at which the Camera will no longer render any mesh. 
	// If you get very close to a mesh, this will sometimes cut the mesh and allow you to see inside of it.
	mCamera->setNearClipDistance(5);
	// Create a new SdkCameraMan. This is the Camera controller provided by OgreBites.
	// Dont forget to delete this
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
 
void TutorialApplication::createViewports()
{
	// Let's create a Viewport for our scene.
	// mWindow is another variable defined for us in BaseApplication.
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	// Let's set the background color of the Viewport.
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	// Set the aspect ratio of our Camera.
	// Default aspect ratio.
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / 
		Ogre::Real(vp->getActualHeight())
	);
}

//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	bool ret = BaseApplication::frameRenderingQueued(fe);

	if(!processUnbufferedInput(fe))
		return false;
	
	toggleTimer -= fe.timeSinceLastFrame;
 
	return ret;
}
 
bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
	static bool mouseDownLastFrame = false;
	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
	static Ogre::Real rotate = .13;
	static Ogre::Real move = 250;

	if (mKeyboard->isKeyDown(OIS::KC_I))
		dirVec.z -= move;
	if (mKeyboard->isKeyDown(OIS::KC_K))
		dirVec.z += move;
	if (mKeyboard->isKeyDown(OIS::KC_U))
		dirVec.y += move;
	if (mKeyboard->isKeyDown(OIS::KC_O))
	{
		dirVec.y -= move;
		Ogre::Real y = mSceneMgr->getSceneNode("entityNode")->getPosition().y;
		if (mSceneMgr->getSceneNode("entityNode")->getPosition().y <= 25.0)
			dirVec.y = 0;
	}

	if (mKeyboard->isKeyDown(OIS::KC_J))
	{    
	  if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		mSceneMgr->getSceneNode("entityNode")->yaw(Ogre::Degree(5 * rotate));
	  else
		dirVec.x -= move;
	}
 
	if (mKeyboard->isKeyDown(OIS::KC_L))
	{
	  if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		mSceneMgr->getSceneNode("entityNode")->yaw(Ogre::Degree(-5 * rotate));
	  else
		dirVec.x += move;
	}

	mSceneMgr->getSceneNode("entityNode")->translate(
		dirVec * fe.timeSinceLastFrame,
		Ogre::Node::TS_LOCAL);

	bool leftMouseDown = mMouse->getMouseState().buttonDown(OIS::MB_Left);
	if (leftMouseDown && !mouseDownLastFrame)
	{

		Ogre::Light* light = mSceneMgr->getLight("SpotLight");
		light->setVisible(!light->isVisible());
	}
	mouseDownLastFrame = leftMouseDown;

	if ((toggleTimer <= 0) && mMouse->getMouseState().buttonDown(OIS::MB_Right))
	{

		toggleTimer  = 0.5;

		Ogre::Light* light = mSceneMgr->getLight("SpotLight");
		light->setVisible(!light->isVisible());
	}


	return true;
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
