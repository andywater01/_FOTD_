#pragma once

#include "Scene.h"

class AssignmentScene : public Scene
{
public:
	AssignmentScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	int GetTrainer();
	int GetCrono();
	int GetCrate();

private:
	float m_trainerEntity;
	float m_cronoEntity;
	float m_crateEntity;
};
