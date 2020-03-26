#pragma once

#include "Scene.h"

class NewScene : public Scene
{
public:
	NewScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
};