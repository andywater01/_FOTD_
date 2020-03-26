#include "Menu.h"
#include "Game.h"


Menu::Menu(std::string name)
	: Scene(name)
{
}



void Menu::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//Camera Entity
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		//ECS::AttachComponent<VerticalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Camera");
		ECS::SetIsMainCamera(entity, true);
	}
	

	{

		
		MenuButton ButtonID;
		auto mapLayout = File::LoadJSON("AllMenu1.json");

		
		
		//Creates entity
		auto globalMap = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(globalMap);
		ECS::AttachComponent<Transform>(globalMap);
		ECS::AttachComponent<AnimationController>(globalMap);


		//Set up components
		std::string globalMapFile = "MenuEverything.png";
		auto& menuAnim = ECS::GetComponent<AnimationController>(globalMap);
		menuAnim.InitUVs(globalMapFile);

		//Adds first animation
		menuAnim.AddAnimation(mapLayout["play"]);	// 0		//0
		menuAnim.AddAnimation(mapLayout["controls"]);	// 1	//1
		menuAnim.AddAnimation(mapLayout["credits"]);	// 2	//3
		menuAnim.AddAnimation(mapLayout["creditnames"]);	// 3	//4
		menuAnim.AddAnimation(mapLayout["controlswasd"]);	// 4	//5
	

		//Sets active animation
		menuAnim.SetActiveAnim(0);
		menuAnim.GetAnimation(0);
		//Gets first animation

		ECS::GetComponent<Sprite>(globalMap).LoadSprite(globalMapFile, 220, 200, true, &menuAnim);
		//ECS::GetComponent<Sprite>(globalMap).SetUVs(vec2(14.f, 34.f), vec2(30.f, 11.f));
		ECS::GetComponent<Transform>(globalMap).SetPosition(vec3(0.f, 0.f, 20.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(globalMap, bitHolder, "Global Map");
		

		m_mainMenu = globalMap;
	}

	
	
	
}

int Menu::Menu1()
{
	return m_mainMenu;
}


