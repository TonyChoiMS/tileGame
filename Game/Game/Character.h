#pragma once

#include <string>
#include <d3dx9.h>
#include "Sprite.h"
#include "GlobalTypes.h"

class Sprite;

class Character
{
private:
	Sprite* _sprite;
	Point _position;

public:
	Character(std::wstring name);
	~Character();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();
	
};