#pragma once

#include "State.h"

//class Character;

class IdleState
	: public State
{

public:
	IdleState(Character* character);
	~IdleState();
public:
	void Start();
	void Update(float deltaTime);


};