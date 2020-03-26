#include "BioshockContactListener.h"
#include "BioshockScene.h"
#include "ECS.h"
#include "Game.h"


BioshockContactListener::BioshockContactListener(entt::registry* reg)
{
	m_register = reg;
}


void BioshockContactListener::BeginContact(b2Contact* contact)
{

	//Creating Fixtures
	auto f1 = contact->GetFixtureA();

	//Creating Bodies
	auto b1 = f1->GetBody();

	//Creating Objects
	unsigned int o1 = (unsigned int)b1->GetUserData();


	//Checking if both objects are still valid
	if (!m_register->valid(o1))
		return;

	//Creating Identifiers
	auto i1 = m_register->get<EntityIdentifier>(o1).GetIdentifier();



	//Creating Fixtures
	auto f2 = contact->GetFixtureB();

	//Creating Bodies
	auto b2 = f2->GetBody();

	//Creating Objects
	unsigned int o2 = (unsigned int)b2->GetUserData();

	//Checking if both objects are valid
	if (!m_register->valid(o2))
		return;


	//Creating Identifiers
	auto i2 = m_register->get<EntityIdentifier>(o2).GetIdentifier();


	bool destroyA = false;
	bool destroyB = false;

	Game sensorCheck;

	//BioshockScene* scene = (BioshockScene*)Scene::GetScene;
	//auto HeartsEntity = scene->GetHealth1();

	//for Hearts
	auto& heartspos = m_register->get<Transform>(EntityIdentifier::MainHeart());
	auto& animHearts = m_register->get<AnimationController>(EntityIdentifier::MainHeart());

	//For Player
	auto& playerAnim = m_register->get<AnimationController>(EntityIdentifier::MainPlayer());
	auto& playerPos = m_register->get<Transform>(EntityIdentifier::MainPlayer());

	//For enemy
	auto& animEnemy = m_register->get<AnimationController>(EntityIdentifier::MainEnemy());
	auto EnemyPos = m_register->get<Transform>(EntityIdentifier::MainEnemy());
	

	if (i1 & EntityIdentifier::PlayerBit())
	{
		if (i2 & EntityIdentifier::EnemyBit())
		{

			std::cout << "Hit Achieved!\n";

			HealthBarnum++;

			animHearts.SetActiveAnim(HealthBarnum);

			//m_register->destroy(o2);

			
		}

		

	}

	// Checks if player attacks enemy from a radius X
		//if ((EnemyPos.GetPositionX() - playerPos.GetPositionX() >= -100 || playerPos.GetPositionX() - EnemyPos.GetPositionX() <= 100) && (Input::GetKeyDown(Key::LeftArrow) || Input::GetKeyDown(Key::RightArrow)))
	if (Input::GetKeyDown(Key::J))
	{
		std::cout << "You hit the Enemy" << std::endl;
		m_register->destroy(o2);
	}

	if (Input::GetKey(Key::RightArrow))
	{

		std::cout << "Right Hit!\n";
		
	}


	/*
	//Creating Fixtures
	auto ff1 = contact->GetFixtureA();

	//Creating Bodies
	auto bb1 = ff1->GetBody();

	//Creating Objects
	unsigned int oo1 = (unsigned int)bb1->GetUserData();


	//Checking if both objects are still valid
	if (!m_register->valid(oo1))
		return;

	//Creating Identifiers
	auto ii1 = m_register->get<EntityIdentifier>(oo1).GetIdentifier();



	//Creating Fixtures
	auto ff2 = contact->GetFixtureB();

	//Creating Bodies
	auto bb2 = ff2->GetBody();

	//Creating Objects
	unsigned int oo2 = (unsigned int)bb2->GetUserData();

	//Checking if both objects are valid
	if (!m_register->valid(oo2))
		return;


	//Creating Identifiers
	auto ii2 = m_register->get<EntityIdentifier>(oo2).GetIdentifier();



	/*
	if (ii1 & EntityIdentifier::AttackRightBit())
	{
		if (ii2 & EntityIdentifier::EnemyBit())
		{
			std::cout << "Right Hit!\n";
		}
	}
	*/
	



}

void BioshockContactListener::CheckAttack()
{
}
