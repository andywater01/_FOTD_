#pragma once

#include "Scene.h"
#include "BioshockContactListener.h"

class BioshockScene : public Scene
{
public:
	BioshockScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	int GetPlayer();
	int GetDiary();
	int GetTrainer();
	int GetBolt();
	int GetBlood1();
	int GetBlood2();
	int GetBlood3();
	int GetBlood4();
	int GetHeart();
	int GetSplicer2();
	int GetSplicer3();
	int GetSplicer4();
	int GetSpeech();
	int GetInteract();
	int GetWin();
	int GetLose();
	int GetSlash();
	int GetHealth1();

	int GetEnemy();

	int GetEnemyArray(int x);

	void spawnEnemies();

	
	int roundnum = 2;
	int numofenemies = roundnum;

	
	

	bool rightAttack = false;

private:
	BioshockContactListener m_listener;
	float m_playerEntity;
	float m_slashEntity;
	float m_diaryEntity;
	float m_trainerEntity;
	float m_boltEntity;
	float m_bloodEntity1;
	float m_bloodEntity2;
	float m_bloodEntity3;
	float m_bloodEntity4;
	float m_heartEntity;
	float m_splicer2Entity;
	float m_splicer3Entity;
	float m_splicer4Entity;
	float m_speechEntity;
	float m_interactEntity;
	float m_winEntity;
	float m_loseEntity;
	float m_Slash;
	float m_enemyArray[100];

	

};