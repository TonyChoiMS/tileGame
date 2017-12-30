#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include "GlobalTypes.h"
#include "Component.h"

class Sprite;

class Character : public Component
{
protected:
	std::vector<Sprite*> _spriteList;
	int _currentDirection;

	TilePoint _tilePosition;

public:
	Character(std::wstring name);
	~Character();

	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	// AI
protected:
	float _moveTime;
	float _movingDuration;
	bool _isMoving;

public:
	virtual void UpdateAI(float deltaTime);
};
