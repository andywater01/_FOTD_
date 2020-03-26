#include "HelloWorld.h"

HelloWorld::HelloWorld(std::string name)
	: Scene(name)
{
}

void HelloWorld::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register (so when you unload the scene when you switch between scenes
	//you can reInit this scene
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera Entity
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to the vertical scroll
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}

	//Setup New Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Creates new orthographic camera
		ECS::AttachComponent<HealthBar>(entity);
		ECS::GetComponent<HealthBar>(entity).SetHealth(0.7f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::HealthBarBit();
		ECS::SetUpIdentifier(entity, bitHolder, "HealthBar Entity");
	}

	//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 70.f, 0.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hello World Sign");
	}

	//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "SoraSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 70);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, -60.f, 101.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Sora Sprite");
	}

	//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "KanekiSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 60);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-60.f, -60.f, 99.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Kaneki Sprite");
	}

	//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "GokuBlackSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 80);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Goku Black Sprite");
	}

	/*//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "ChronoSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 74.f), vec2(47.f, 0.f));
		anim.AddFrame(vec2(47.f, 74.f), vec2(94.f, 0.f));
		anim.AddFrame(vec2(94.f, 74.f), vec2(141.f, 0.f));
		anim.AddFrame(vec2(141.f, 74.f), vec2(188.f, 0.f));
		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Chrono Sprite Sheet");
	}*/


	//Setup New Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		EntityIdentifier::MainPlayer(entity);


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "ChronoRight.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);

		//Attack Down
		anim.AddFrame(vec2(1.f, 65.f), vec2(65.f, 1.f));
		anim.AddFrame(vec2(67.f, 65.f), vec2(131.f, 1.f));

		//Attack Left
		anim.AddFrame(vec2(133.f, 65.f), vec2(197.f, 1.f));
		anim.AddFrame(vec2(199.f, 65.f), vec2(263.f, 1.f));

		//Attack Right
		//anim.AddFrame(vec2(265.f, 65.f), vec2(329.f, 1.f));
		//anim.AddFrame(vec2(331.f, 65.f), vec2(395.f, 1.f));

		//Attack Up
		//anim.AddFrame(vec2(397.f, 65.f), vec2(461.f, 1.f));
		//anim.AddFrame(vec2(463.f, 65.f), vec2(527.f, 1.f));
		
		//Cast Down
		//anim.AddFrame(vec2(529.f, 65.f), vec2(593.f, 1.f));

		//Cast Left
		//anim.AddFrame(vec2(595.f, 65.f), vec2(659.f, 1.f));

		//Cast Right
		//anim.AddFrame(vec2(661.f, 65.f), vec2(725.f, 1.f));

		//Cast Up
		//anim.AddFrame(vec2(727.f, 65.f), vec2(791.f, 1.f));

		//WalkDown
		//anim.AddFrame(vec2(793.f, 65.f), vec2(857.f, 1.f));
		//anim.AddFrame(vec2(859.f, 65.f), vec2(923.f, 1.f));
		//anim.AddFrame(vec2(925.f, 65.f), vec2(989.f, 1.f));
		//anim.AddFrame(vec2(991.f, 65.f), vec2(1055.f, 1.f));

		//Hurt
		//anim.AddFrame(vec2(1057.f, 65.f), vec2(1121.f, 1.f));

		//Idle
		//anim.AddFrame(vec2(1123.f, 65.f), vec2(1187.f, 1.f));

		//WalkLeft
		//anim.AddFrame(vec2(1189.f, 65.f), vec2(1253.f, 1.f));
		//anim.AddFrame(vec2(1255.f, 65.f), vec2(1319.f, 1.f));
		//anim.AddFrame(vec2(1321.f, 65.f), vec2(1385.f, 1.f));
		//anim.AddFrame(vec2(1387.f, 65.f), vec2(1451.f, 1.f));

		//WalkRight
		//anim.AddFrame(vec2(1453.f, 65.f), vec2(1517.f, 1.f));
		//anim.AddFrame(vec2(1519.f, 65.f), vec2(1583.f, 1.f));
		//anim.AddFrame(vec2(1585.f, 65.f), vec2(1649.f, 1.f));
		//anim.AddFrame(vec2(1651.f, 65.f), vec2(1715.f, 1.f));

		//Walk Up
		//anim.AddFrame(vec2(1717.f, 65.f), vec2(1781.f, 1.f));
		//anim.AddFrame(vec2(1783.f, 65.f), vec2(1847.f, 1.f));
		//anim.AddFrame(vec2(1849.f, 65.f), vec2(1913.f, 1.f));
		//anim.AddFrame(vec2(1915.f, 65.f), vec2(1979.f, 1.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Chrono Walk Right");
		
		
	}

	//Makes the camera focus on the main player
	//We do this at the very bottom so we get the most accurate pointer to our Transform
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

}
