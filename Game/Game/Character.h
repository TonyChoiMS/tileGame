#pragma once

#include <string>
#include <d3dx9.h>
#include "Sprite.h"

class Sprite;

class Character
{
private:
	Sprite* _sprite;
	float _x;
	float _y;

public:
	Character(std::wstring name);
	~Character();

	void Init(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
	
};