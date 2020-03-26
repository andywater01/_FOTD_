#pragma once

#include "Box2D/Box2D.h"
#include "entt/entity/registry.hpp"

#include "PhysicsBody.h"
#include "Timer.h"
#include "Shader.h"
#include "Input.h"
#include "Game.h"
#include "Scene.h"


class BioshockContactListener : public b2ContactListener
{
public:
	BioshockContactListener() { };
	BioshockContactListener(entt::registry* reg);

	void BeginContact(b2Contact* contact) override;

	void CheckAttack();


private:
	entt::registry* m_register = nullptr;

	float m_enemy;

	static Shader physicsDrawShader;

	int HealthBarnum = 0;
};
