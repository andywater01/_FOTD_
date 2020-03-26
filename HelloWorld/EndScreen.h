#pragma once
#include "Scene.h"

class EndScreen : public Scene
{
public:
	EndScreen(std::string name);

	virtual void InitScene(float windowWidth, float windowHeight);
};

