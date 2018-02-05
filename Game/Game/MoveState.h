#pragma once

#include "State.h"


class MoveState
	: public State
{
private:
	//Character* _character;

private:
	//bool _isMoving;
	//float _moveTime;
	float _movingDuration;
public:
	MoveState(Character* character);
	~MoveState();
public:
	void Update(float deltaTime);
	void UpdateMove(float deltaTime);

	void Start();
	void Stop();

	//void SetMoving(bool isMoving) { _isMoving = isMoving; }
	//bool IsMoving() { return _isMoving; }
	float GetMovingDuration() { return _movingDuration; }
};