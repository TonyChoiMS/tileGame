#pragma once

#include <string>
#include <d3dx9.h>
#include <vector>

#include "Component.h"
#include "GlobalTypes.h"

#define mapWidth 16
#define mapHeight 16

class Sprite;
class TileCell;

class Map : public Component
{
private:
	std::vector<Sprite*> _spriteList;
	
	// ���� ��������Ʈ �迭 ���, Ÿ�ϼ� �迭�� ����.
	std::vector<std::vector<TileCell*>> _tileArray;
	//std::vector<std::vector<Sprite*>> _spriteArray;

	int _width;
	int _height;

	int _renderWidth;
	int _renderHeight;

	int _tileSize;

public:
	Map(std::wstring name);
	~Map();

	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	

	void Release();
	void Reset();
	//Map Info
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	// Viewer
private:
	Component* _viewer;
	TilePoint _prevViewTilePosition;
public:
	void SetViewer(Component* component);
	void UpdateViewer(float deltaTime);

	//Move
private:
	int _startX;
	int _startY;

public:
	void SetTileComponent(TilePoint tilePosition, Component* component);
	void ResetTileComponent(TilePoint tilePosition, Component* component);
	Point GetPosition(int tileX, int tileY);

	bool CanMoveTile(TilePoint tilePosition);
	TileCell* GetTileCell(TilePoint tilePosition);

	Component* FindComponentInRange(Component* finder, int range, std::vector<eComponentType> findTypeList);
	std::vector<Component*> GetTileCollisionList(TilePoint tilePosition);

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();	
};