#pragma once
#include "State.h"

class Character;

class PathfindingMoveState : public State
{
private:
	float _movingDuration;

public:
	PathfindingMoveState(Character* character);
	~PathfindingMoveState();

public:
	void Start();
	void Stop();
	void Update(float deltaTime);

	eDirection GetDirection(TilePoint toPosition, TilePoint fromPosition);
};