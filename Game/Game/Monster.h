#pragma once
#include "Character.h"
#include <list>
#include "ComponentMessage.h"


class Monster
	: public Character
{
public:
	Monster(std::wstring name);
	~Monster();

public:
	void UpdateAI(float deltaTime);

	void Collision(std::vector<Component*> collisionList);
};