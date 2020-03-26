#pragma once
#include "Scene.h"



class Menu : public Scene
{
public:
	Menu(std::string name);

	virtual void InitScene(float windowWidth, float windowHeight);
	int Menu1();
	

private:
	int m_mainMenu;
	int m_credControls;
};

struct MenuButton
{
	int buttonID;

};

