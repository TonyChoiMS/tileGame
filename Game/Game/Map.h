#pragma once

#include <string>
#include <d3dx9.h>
#include "Sprite.h"

#define mapWidth 64
#define mapHeight 64

class Sprite;

class Map
{
private:
	Sprite* _spriteArray[mapHeight][mapWidth];

	int _height;
	int _width;

	int _renderWidth;
	int _renderHeight;

public:
	Map(std::wstring name);
	~Map();

	void Init(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
	void Deinit();
	void Update(int deltaTime);
	void Render();
	

	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
};