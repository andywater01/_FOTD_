#ifndef __GAME_H__
#define __GAME_H__

#include "BackEnd.h"
#include "HelloWorld.h"
#include "NewScene.h"
#include "Xinput.h"
#include "AssignmentScene.h"
#include "EndScreen.h"

//Our main class for running our game
class Game
{
public:
	//Empty constructor
	Game() { };
	//Deconstructor for game
	//*Unloads window
	~Game();

	//Initiaiizes game
	//*Seeds random
	//*Initializes SDL
	//*Creates Window
	//*Initializes GLEW
	//*Create Main Camera Entity
	//*Creates all other entities and adds them to register
	void InitGame();
	void InitVariables();

	//Runs the game
	//*While window is open
	//*Clear window
	//*Update 
	//*Draw
	//*Poll events
	//*Flip window
	//*Accept input
	bool Run();
	
	//Updates the game
	//*Update timer
	//*Update the rendering system
	//*Update the animation system
	void Update();

	//Runs the GUI
	//*Uses ImGUI for this
	void GUI();

	//Check events
	//*Checks the results of the events that have been polled
	void CheckEvents();
	
	/*Input Functions*/
	void AcceptInput();
	void GamepadInput();

	void GamepadStroke(XInputController* con);
	void GamepadUp(XInputController* con);
	void GamepadDown(XInputController* con);
	void GamepadStick(XInputController* con);
	void GamepadTrigger(XInputController* con);
	void KeyboardHold();
	void KeyboardDown();
	void KeyboardUp();

	//Mouse input
	void MouseMotion(SDL_MouseMotionEvent evnt);
	void MouseClick(SDL_MouseButtonEvent evnt);
	void MouseWheel(SDL_MouseWheelEvent evnt);
	void AttackMouseClick(SDL_MouseWheelEvent evnt);

	//Create Menu Button
	void CreateMenuButton();
	void CreateEnemy();
	
	
	

	//creates slash sound effect
	void SlashSound();
	void Hearts();
	void UpdateEnemy();
	void UpdateTime();

	//Hearts
	void setHearts(int hearts);

	int getHearts();
	
	int getEnemyNum();

private:
	//The window
	Window *m_window = nullptr;

	//Scene name
	std::string m_name;
	//Clear color for when we clear the window
	vec4 m_clearColor;
	
	//The main register for our ECS
	entt::registry* m_register;

	//Scenes
	Scene* m_activeScene;
	std::vector<Scene*> m_scenes;
	
	//Imgui stuff
	bool m_guiActive = false;

	//Hooks for events
	bool m_close = false;
	bool m_motion = false;
	bool m_click = false;
	bool m_wheel = false;

	//Velocity
	vec2 m_velocity = vec2(0.f, 0.f);
	//Mass
	float m_mass = 15.f;

	//Bolt Bool
	int m_boltTrigger = 1;

	//Health Timer
	float m_healthTimer = 0.f;
	float m_healthHit = 1.5f;

	//Hit Timer
	float m_hitTimer = 0.f;
	float m_enemyHit = 0.001f;

	//Player Health
	float m_playerHealth = 0.f;

	int m_healthBarNum = 0;
	int EnemyNum = 0;
};


#endif // !__GAME_H__

