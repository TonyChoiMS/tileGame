#pragma once

#include <string>
#include <d3dx9.h>
#include "Sprite.h"
#include "GlobalTypes.h"
#include "Component.h"

class Sprite;

class Character : public Component
{
private:
	Sprite* _sprite;
	TilePoint _tilePosition;

public:
	Character(std::wstring name);
	~Character();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	// Move AI
private:
	float _movingDuration;
	float _moveTime;
	bool _isMoving;

public:
	void UpdateAI(float deltaTime);
	
};