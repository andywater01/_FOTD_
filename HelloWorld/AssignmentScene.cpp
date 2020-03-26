#include "AssignmentScene.h"

AssignmentScene::AssignmentScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-9.f)); //Set Gravity (only second value)
	m_physicsWorld->SetGravity(m_gravity);
}

void AssignmentScene::InitScene(float windowWidth, float windowHeight)
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
		ECS::AttachComponent<VerticalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to the horizontal scroll
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);

		//Attaches the camera to the vertical scroll
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal and Vertical Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}

	//Sets up ground
	{
		//Creates New Entity
		auto entity = ECS::CreateEntity();
		//Attaches Sprite and Tranform components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Loads in the second big sprite sheet
		std::string fileName = "BG.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 615, 228);
		//Gets the transform component and sets the position to middle of the screen
		//(Underneath everything so that it acts as a background)
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -10.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the bottom of the sprite (so that only the ground part of the sprite acts as environment)
		//ID type is Environment
		//Collides with nothing (as it doesn't move, thus other things collide with it)
		float shrinkX = 0.f;
		float shrinkY = (tempSpr.GetHeight() / 2.f);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, (-tempSpr.GetHeight() / 16.f) * 6.f), false);
		

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Ground");
	}

	/*
	//Setup Background Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "Background3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 400);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30.f, -100.f, 0.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");
	}
	*/

	/*
	//Setup Flower 1 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("FlowerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FlowerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Flowering"]);
		animController.GetAnimation(0).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-45.f, 10.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Flower");

	}

	//Setup Flower 2 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("FlowerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FlowerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Flowering"]);
		animController.GetAnimation(0).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-45.f, -18.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Flower");

	}

	//Setup Flower 3 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("FlowerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FlowerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Flowering"]);
		animController.GetAnimation(0).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(25.f, -85.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Flower");

	}

	//Setup Flower 4 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("FlowerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FlowerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Flowering"]);
		animController.GetAnimation(0).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, -85.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Flower");

	}

	//Setup Flower 4 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("FlowerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FlowerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Flowering"]);
		animController.GetAnimation(0).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(75.f, -85.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Flower");

	}

	//Setup Fence1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));
		
		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-38.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup Fence2 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup Fence3 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(18.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup Fence4 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(46.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup Fence5 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(74.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup Fence6 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Fence.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(102.f, 50.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence");
	}

	//Setup FencePost1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FencePost.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(78.f, 10.f, 3.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence Post");
	}

	//Setup FencePost2 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FencePost.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(78.f, -10.f, 4.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence Post");
	}

	//Setup FencePost3 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "FencePost.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(78.f, -30.f, 5.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Fence Post");
	}
	*/

	/*//Setup Dio Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "DioSprite.gif";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 120);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-70.f, -25.f, 1.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Dio");
	}*/

	/*
	//Setup Crono Walking Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("CronoAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		EntityIdentifier::MainPlayer(entity);

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "CronoSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Idle"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(1).SetRepeating(true);
		animController.AddAnimation(animation["WalkLeft"]);
		animController.GetAnimation(2).SetRepeating(true);
		animController.AddAnimation(animation["WalkRight"]);
		animController.GetAnimation(3).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(4).SetRepeating(true);

		
		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;
		
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() |  EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Crono");

		m_cronoEntity = entity;
	}
	*/
	

	//Setup Main Crate Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		EntityIdentifier::MainPlayer(entity);

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(50.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Crate");

		m_crateEntity = entity;
	}


	//Setup Crate1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(60.f), float32(50.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup Crate2 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(95.f), float32(50.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup Crate3 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(130.f), float32(50.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup Crate4 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(165.f), float32(50.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup R2Crate1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(78.f), float32(100.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup R2Crate2 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(114.f), float32(100.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup R2Crate3 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(150.f), float32(100.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup R3Crate1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(95.f), float32(150.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}


	//Setup R3Crate2 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(135.f), float32(150.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}

	//Setup R4Crate1 Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "Crate.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 643.f), vec2(646.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(115.f), float32(200.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "SmallCrate");

	}

	/*
	//Setup Trainer Walking Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("TrainerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "TrainerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Idle"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(1).SetRepeating(true);
		animController.AddAnimation(animation["WalkLeft"]);
		animController.GetAnimation(2).SetRepeating(true);
		animController.AddAnimation(animation["WalkRight"]);
		animController.GetAnimation(3).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(4).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-75.f, -50.f, 40.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Trainer");

		m_trainerEntity = entity;
	}
	*/
	//Makes the camera focus on the main player
	//We do this at the very bottom so we get the most accurate pointer to our Transform
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

}


//Get Entities
int AssignmentScene::GetTrainer()
{
	return m_trainerEntity;
}

int AssignmentScene::GetCrono()
{
	return m_cronoEntity;
}

int AssignmentScene::GetCrate()
{
	return m_crateEntity;
}