#pragma once

#include "State.h"


class DieState
	: public State
{

public:
	DieState(Character* character);
	~DieState();
public:
	void Start();
	void Update(float deltaTime);

	// ������
	void CreateSprite(std::wstring textureFilename, std::wstring scriptFilename);
};