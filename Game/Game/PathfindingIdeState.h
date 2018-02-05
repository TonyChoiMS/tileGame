#pragma once
#include "State.h"

class PathfindingIdeState
	:public State
{
public:
	PathfindingIdeState(Character* character);
	~PathfindingIdeState();

public:
	void Update(float deltaTime);
};