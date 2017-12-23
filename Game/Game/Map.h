#pragma once

#include <string>
#include <d3dx9.h>
#include <vector>
#include "Sprite.h"

#define mapWidth 16
#define mapHeight 16

class Sprite;

class Map
{
private:
	std::vector<Sprite*> _spriteList;

	std::vector<std::vector<Sprite*>> _spriteArray;

	int _height;
	int _width;

	int _renderWidth;
	int _renderHeight;

public:
	Map(std::wstring name);
	~Map();

	void Init();
	void Deinit();
	void Update(int deltaTime);
	void Render();
	

	void Release();
	void Reset();

private:
	int _startX;
	int _startY;

public:
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
};