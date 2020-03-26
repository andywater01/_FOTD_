#include "Game.h"
#include "BackEnd.h"
#include "HelloWorld.h"
#include "NewScene.h"
#include "AssignmentScene.h"
#include "BioshockScene.h"
#include "Menu.h"
#include <iostream>
#include "GPCSound.h"

#include <windows.h>
#include <mmsystem.h>
#include <ctime>


#pragma comment(lib, "winmm.lib")

#include <random>

using namespace std;

bool weapon = false;
int menuchoice = 0;
int scenenum = 0;

int roundnum = 1;
int numofenemies = 0;

int jumpcount = 0;

int Enemyxpos;
int Enemyypos;

int seconds = 0;


SDL_MouseWheelEvent evntx;

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//Set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Scene names and clear colors
	m_name = "Fury of the Death";
	m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	//Initializes the backend
	BackEnd::InitBackEnd(m_name);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new HelloWorld scene
	m_scenes.push_back(new Menu("Menu Screen"));
	m_scenes.push_back(new BioshockScene("Bioshock Scene"));
	m_scenes.push_back(new HelloWorld("HorizontalScrolling"));
	m_scenes.push_back(new NewScene("New Scene"));
	m_scenes.push_back(new HelloWorld("Second Loaded Scene"));
	m_scenes.push_back(new AssignmentScene("Assignment Scene"));
	m_scenes.push_back(new EndScreen("EndScreen"));
	

	//Sets active scene reference to our HelloWorld scene
	m_activeScene = m_scenes[scenenum];


	//Initializes the scene
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	//*m_activeScene = File::LoadJSON("Hello World.json");

	//Sets m_register to point to the register in the active scene
	m_register = m_activeScene->GetScene();

	//InitVariables();

	PhysicsSystem::Init();

	std::cout << "Controller 1: " << (XInputManager::ControllerConnected(0) ? "Connected" : "Not Connected") << std::endl;
}


bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with clearColor
		m_window->Clear(m_clearColor);
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();
		
		//Flips the windows
		m_window->Flip();
		
		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	if (scenenum == 1)
	{
		//Update Physics System
		PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());

		BioshockScene* scene = (BioshockScene*)m_activeScene;

		auto HeartEntity = scene->GetHealth1();
		auto& HeartAnim = m_register->get<AnimationController>(HeartEntity);
		auto HeartPos = m_register->get<Transform>(HeartEntity);

		m_register->get<Transform>(HeartEntity).SetPosition(m_register->get<Transform>(EntityIdentifier::MainPlayer()).GetPosition() + vec3(-5.f, 30.f, 0.f));
		
	}
	

	//Updates Scene
	m_activeScene = m_scenes[scenenum];

}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
	{
		MouseClick(BackEnd::GetClickEvent());
		//AttackMouseClick(evntx);
	}
		

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());

	


	if (scenenum == 0)
	{
		CreateMenuButton(); // Creates the menu
		
	}

	if (scenenum == 1)
	{
		Hearts();
		UpdateEnemy();
		if (scenenum != 6)
		{
			//UpdateTime();
		}
		
		if (numofenemies < 2)
		{
			//CreateEnemy();
		}
		
		auto& heartspos = m_register->get<Transform>(EntityIdentifier::MainHeart());
		auto& animHearts = m_register->get<AnimationController>(EntityIdentifier::MainHeart());

		if (animHearts.GetActiveAnim() == 6)
		{
			scenenum = 6;
			m_activeScene = m_scenes[scenenum];
			m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
			m_register = m_activeScene->GetScene();
		}

	}

	

}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			//If controller is connected, we run the different input types
			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}


void Game::KeyboardHold()
{
	

	if (scenenum == 1)
	{
		SoundManager::init("./Assets/Sounds/");
		Sound2D _Slash("SwordSlash.wav", "group1");

		BioshockScene* scene = (BioshockScene*)m_activeScene;

		auto playerEntity = scene->GetPlayer();
		auto& playerPhys = m_register->get<PhysicsBody>(playerEntity);
		auto& playerAnim = m_register->get<AnimationController>(playerEntity);
		auto& playerPos = m_register->get<Transform>(EntityIdentifier::MainPlayer());

		auto HeartEntity = scene->GetHealth1();
		auto& HeartAnim = m_register->get<AnimationController>(HeartEntity);
		auto HeartPos = m_register->get<Transform>(HeartEntity);


		vec2 totalForce = vec2(0.f, 0.f); //(0.f, -1.f) <--Imitates gravity


		float speed = 50.f;


		//Keyboard button held
		if (Input::GetKey(Key::W))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(13);

			playerPhys.ApplyForce((vec3(0.f, 500000.f, 0.f))* (speed * Timer::deltaTime));

			//ECS::GetComponent<Transform>(HeartEntity).SetPositionY((HeartPos.y - 0.25f) + (speed * Timer::deltaTime));

			//auto& trans = m_register->get<Transform>(EntityIdentifier::MainPlayer());
			//trans.SetPositionY(trans.GetPositionY() + (speed * Timer::deltaTime));
		}
		if (Input::GetKey(Key::A))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(11);


			playerPhys.ApplyForce((vec3(-500000.f, 0.f, 0.f))* (speed * Timer::deltaTime));


		}
		if (Input::GetKey(Key::S))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(8);

			playerPhys.ApplyForce((vec3(0.f, -500000.f, 0.f))* (speed * Timer::deltaTime));



		}
		if (Input::GetKey(Key::D))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(12);

			playerPhys.ApplyForce((vec3(500000.f, 0.f, 0.f))* (speed * Timer::deltaTime));

		}

		if (jumpcount < 500)
		{
			vec3 position = m_register->get<Transform>(EntityIdentifier::MainPlayer()).GetPosition();
			if (Input::GetKey(Key::Space) && Input::GetKey(Key::D)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce((vec3(5000000.f, 0.f, 0.f))* (speed * Timer::deltaTime));
				playerAnim.SetActiveAnim(3);
				jumpcount++;
			}
			if (Input::GetKey(Key::Space) && Input::GetKey(Key::A)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce((vec3(-5000000.f, 0.f, 0.f))* (speed * Timer::deltaTime));
				playerAnim.SetActiveAnim(1);
				jumpcount++;
			}

			if (Input::GetKey(Key::Space) && Input::GetKey(Key::W)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce((vec3(0.f, 5000000.f, 0.f))* (speed * Timer::deltaTime));
				playerAnim.SetActiveAnim(2);
				jumpcount++;
			}

			if (Input::GetKey(Key::Space) && Input::GetKey(Key::S)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce((vec3(0.f, -5000000.f, 0.f))* (speed * Timer::deltaTime));
				playerAnim.SetActiveAnim(4);
				jumpcount++;
			}
			
		}




		//Melee Attacks
		if (Input::GetKeyDown(Key::RightArrow))
		{
			_Slash.play();
			playerAnim.SetActiveAnim(2);
		}
		if (Input::GetKeyDown(Key::LeftArrow))
		{
			if (weapon == false)
			{
				
				_Slash.play();
				playerAnim.SetActiveAnim(1);

			}
		}
		if (Input::GetKeyDown(Key::DownArrow))
		{
			if (weapon == false)
			{
				
				_Slash.play();
				playerAnim.SetActiveAnim(0);

			}
		}
		if (Input::GetKeyDown(Key::UpArrow))
		{
			if (weapon == false)
			{
				
				_Slash.play();
				playerAnim.SetActiveAnim(3);

			}
		}
		

		

		//Mass
		float m_mass = 0.5f;
	}
	

	
	
}

void Game::KeyboardDown()
{
	

	
}

void Game::KeyboardUp()
{
	if (scenenum == 1)
	{
		if (Input::GetKeyUp(Key::P))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
	}


	if (Input::GetKeyUp(Key::F1))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}

	
	

}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//printf("Mouse Moved (%f, %f)\n", float(evnt.x), float(evnt.y));





	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}
	if (SDL_GetMouseState(NULL, NULL))
	{
		printf("Mouse Moved at (%f, %f)\n", float(evnt.x), float(evnt.y));
	}

	

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	
	if (scenenum == 1)
	{

		std::string fileName = "slash.png";
		BioshockScene* slash = (BioshockScene*)m_activeScene;
		auto SlashEntity = slash->GetSlash();
		auto& Slashposition = m_register->get<Transform>(SlashEntity);
		auto& animSlash = m_register->get<AnimationController>(SlashEntity);


		SoundManager::init("./Assets/Sounds/");
		Sound2D _Slash("SwordSlash.wav", "group1");
		
		if (scenenum == 1 && SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			 //Sets Size of Sprites
			Slashposition.SetPositionX(float(evnt.x));
			Slashposition.SetPositionY(float(evnt.y));
			Slashposition.SetPositionZ(float(70));

			//ECS::GetComponent<Transform>(EntityIdentifier::Slash()).SetPosition(vec3(evnt.x, evnt.y, 60.f));  //Sets Location

			animSlash.SetActiveAnim(0);



			_Slash.play();
		}


		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			

			//PlaySound(TEXT("SwordSlash.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);
			
		}


		BioshockScene* scene = (BioshockScene*)m_activeScene;

		auto playerEntity = scene->GetPlayer();
		auto& playerPhys = m_register->get<PhysicsBody>(playerEntity);
		auto& playerAnim = m_register->get<AnimationController>(playerEntity);
		auto& playerPos = m_register->get<Transform>(EntityIdentifier::MainPlayer());

		// Plays animation when clicking mouse
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::D))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			playerAnim.SetActiveAnim(4);
		}
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::A))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(1);

			}
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::S))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(0);

			}
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::W))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(3);

			}
		}




		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			printf("Right Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			printf("Middle Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));
		}


		if (m_guiActive)
		{
			ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
			ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
			ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
			ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
		}

		//Resets the enabled flag
		m_click = false;
	}

	}
	

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	printf("Mouse Scroll %f\n", float(evnt.y));

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}

void Game::AttackMouseClick(SDL_MouseWheelEvent evnt)
{
	std::string fileName = "slash.png";
	BioshockScene* slash = (BioshockScene*)m_activeScene;
	auto SlashEntity = slash->GetSlash();
	auto& Slashposition = m_register->get<Transform>(SlashEntity);
	auto& animSlash = m_register->get<AnimationController>(SlashEntity);


	SoundManager::init("./Assets/Sounds/");
	Sound2D _Slash("SwordSlash.wav", "group1");
	evntx = evnt;
	if (scenenum == 1 && SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		ECS::GetComponent<Sprite>(SlashEntity).LoadSprite(fileName, 50, 50, true, &animSlash); //Sets Size of Sprites
		Slashposition.SetPositionX(float(evnt.x));
		Slashposition.SetPositionY(float(evnt.y));
		Slashposition.SetPositionZ(float(70));

		//ECS::GetComponent<Transform>(EntityIdentifier::Slash()).SetPosition(vec3(evnt.x, evnt.y, 60.f));  //Sets Location

		animSlash.SetActiveAnim(0);
		
		
		
		_Slash.play();
	}
}

void Game::CreateMenuButton()
{
	Menu* scene = (Menu*)m_activeScene;
	auto menuEntity = scene->Menu1();
	vec3 Menuposition = m_register->get<Transform>(menuEntity).GetPosition();
	auto& animController = ECS::GetComponent<AnimationController>(menuEntity);

	
	


	m_activeScene = 0;

	if (Input::GetKeyUp(Key::UpArrow) && m_activeScene == 0) {
		//change imagecount to -1 
		menuchoice--;
		if (menuchoice < 0) {
			menuchoice = 2;
		}
		animController.SetActiveAnim(menuchoice);

	}

	if (Input::GetKeyUp(Key::DownArrow) && m_activeScene == 0) {
		//change imagecount to +1
		menuchoice++;
		if (menuchoice > 2) {
			menuchoice = 0;
		}

		animController.SetActiveAnim(menuchoice);
		


	}

	if (Input::GetKeyUp(Key::Enter) && menuchoice == 0)
	{
		scenenum = 1;

		m_activeScene = m_scenes[scenenum];

		m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
		m_register = m_activeScene->GetScene();
	}

	if (Input::GetKeyUp(Key::Enter) && menuchoice == 1)
	{
		
		

		animController.SetActiveAnim(4);


	}


	if (Input::GetKeyUp(Key::Enter) && menuchoice == 2)
	{
		
		animController.SetActiveAnim(3);

	}

	// Escapes from selection back to menu
	if (Input::GetKeyUp(Key::Escape))
	{

		animController.SetActiveAnim(menuchoice);

	}


}

void Game::CreateEnemy()
{
	for (int i = 0; i < 1; i++)
	{
		float Y;

		Y = rand() % 140 + (-65);

		float X;

		X = rand() % 140 + (-65);

		auto animation11 = File::LoadJSON("BossAnimations.json");
		/*BioshockScene* scene = (BioshockScene*)m_activeScene;
		auto EnemyEntity = scene->GetEnemy();
		auto& Enemypos = m_register->get<Transform>(EnemyEntity);
		auto& animEnemy = m_register->get<AnimationController>(EnemyEntity);*/

		auto Enentity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(Enentity);
		ECS::AttachComponent<Transform>(Enentity);
		ECS::AttachComponent<AnimationController>(Enentity);
		

		std::string fileName = "BossSpriteSheet.png";
		auto& animController11 = ECS::GetComponent<AnimationController>(Enentity);
		animController11.InitUVs(fileName);

		//Adds first animation
		animController11.AddAnimation(animation11["WalkDown"]);
		animController11.GetAnimation(0).SetRepeating(true);
		animController11.AddAnimation(animation11["WalkUp"]);
		animController11.GetAnimation(1).SetRepeating(true);


		//Sets active animation
		animController11.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(Enentity).LoadSprite(fileName, 60, 60, true, &animController11); //Sets Size of Sprites

		ECS::GetComponent<Transform>(Enentity).SetPosition(vec3(X, Y, 45.f));  //Sets Location

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(Enentity, bitHolder, "Boss");

		numofenemies++;
		//BioshockScene::m_enemyArray[j] = entity;

	}
	
}





void Game::Hearts()
{

		auto& heartspos = m_register->get<Transform>(EntityIdentifier::MainHeart());
		auto& animHearts = m_register->get<AnimationController>(EntityIdentifier::MainHeart());



		if (Input::GetKeyUp(Key::T))
		{
			m_healthBarNum++;

			animHearts.SetActiveAnim(m_healthBarNum);

		}

		if (Input::GetKeyUp(Key::Y))
		{
			m_healthBarNum = 0;

			animHearts.SetActiveAnim(m_healthBarNum);
		}

		if (Input::GetKeyDown(Key::W))
		{
			//ECS::GetComponent<Transform>(HeartsEntity).SetPositionX(heartspos.xPos);
		}

		
	
	
}












void Game::GamepadStroke(XInputController * con)
{
	

	
}

void Game::GamepadUp(XInputController * con)
{
	//Gamepad button up
	if (con->IsButtonReleased(Buttons::RB))
	{
		//printf("RB Released\n");
	}
}

void Game::GamepadDown(XInputController * con)
{
	//Gamepad button down
	if (con->IsButtonPressed(Buttons::THUMB_LEFT))
	{
		printf("Left Thumbstick Clicked In.\n");
	}
}

void Game::GamepadStick(XInputController * con)
{
	

}

void Game::GamepadTrigger(XInputController * con)
{
	//Gamepad trigger stuffs
	Triggers triggers;
	con->GetTriggers(triggers);

	if (triggers.RT > 0.5f && triggers.RT < 0.8f)
	{
		printf("Half Press\n");
	}

	if (triggers.RT > 0.8f)
	{
		printf("Full Press\n");
	}

}


// Get Enemy to follow player
void Game::UpdateEnemy()
{
	
	for (int x = 0; x < 2; x++)
	{
		BioshockScene* scene1 = (BioshockScene*)m_activeScene;
		auto Enemyentity = scene1->GetEnemyArray(x);
		auto& EnemyPhys = m_register->get<PhysicsBody>(Enemyentity);
		auto EnemyPos = m_register->get<Transform>(Enemyentity);
		auto& animEnemy = m_register->get<AnimationController>(Enemyentity);

		auto playerEntity = scene1->GetPlayer();
		auto& playerPhys = m_register->get<PhysicsBody>(playerEntity);
		auto& playerAnim = m_register->get<AnimationController>(playerEntity);
		auto playerPos = m_register->get<Transform>(playerEntity);




		int speed = 30;


		if (EnemyPos.GetPositionX() > playerPos.GetPositionX())
		{

			EnemyPhys.ApplyForce((vec3(-300000.f, 0.f, 0.f)) * (speed * Timer::deltaTime));
			animEnemy.SetActiveAnim(1);
			//ECS::GetComponent<Transform>(Enemyentity).SetPositionX((EnemyPos.x - 0.25f) + (speed * Timer::deltaTime));
		}

		if (EnemyPos.GetPositionX() < playerPos.GetPositionX())
		{
			EnemyPhys.ApplyForce((vec3(300000.f, 0.f, 0.f)) * (speed * Timer::deltaTime));
			animEnemy.SetActiveAnim(0);
			//ECS::GetComponent<Transform>(Enemyentity).SetPositionX((EnemyPos.x + 0.25f) + (speed * Timer::deltaTime));
		}

		if (EnemyPos.GetPositionY() > playerPos.GetPositionY())
		{
			EnemyPhys.ApplyForce((vec3(0.f, -300000.f, 0.f)) * (speed * Timer::deltaTime));
			if (EnemyPos.GetPositionX() >= playerPos.GetPositionX())
			{
				animEnemy.SetActiveAnim(1);
			}
			if (EnemyPos.GetPositionX() < playerPos.GetPositionX())
			{
				animEnemy.SetActiveAnim(0);
			}
			//ECS::GetComponent<Transform>(Enemyentity).SetPositionY((EnemyPos.y - 0.25f) + (speed * Timer::deltaTime));
		}

		if (EnemyPos.GetPositionY() < playerPos.GetPositionY())
		{
			EnemyPhys.ApplyForce((vec3(0.f, 300000.f, 0.f)) * (speed * Timer::deltaTime));
			if (EnemyPos.GetPositionX() >= playerPos.GetPositionX())
			{
				animEnemy.SetActiveAnim(1);
			}
			if (EnemyPos.GetPositionX() < playerPos.GetPositionX())
			{
				animEnemy.SetActiveAnim(0);
			}
			//ECS::GetComponent<Transform>(Enemyentity).SetPositionY((EnemyPos.y + 0.25f) + (speed * Timer::deltaTime));
		}
	}
	

}

void Game::UpdateTime()
{
	
	seconds++;
		Sleep(1000);
		cout << seconds << endl;
	
}

void Game::setHearts(int hearts)
{
	m_healthBarNum = hearts;
}

int Game::getHearts()
{
	return m_healthBarNum;
}

int Game::getEnemyNum()
{
	return EnemyNum;
}




/* We found how to use sound here -> https://adamtcroft.com/playing-sound-with-sdl-c/ */