#pragma once

#include <vector>
#include <string>
#include <d3dx9.h>

#include "Component.h"
#include "GlobalTypes.h"

#define mapWidth  16
#define mapHeight 16

class Sprite;
class TileCell;


class Map
	: public Component
{
private:
	Component* _viewer;
	_TilePoint _prevViewTilePosition;
private:
	int _width;
	int _height;
	int _renderMapWidth;
	int _renderMapHeight;
	int _startX;
	int _startY;
	int _tileSize;
	// Ÿ�ϸ� �ε����� ����ϴ� �κ���, ũ�Ⱑ ���������� �ٲ� �� �ֵ��� ����
	//std::vector<std::vector<Sprite*>> _spriteArray;		// 2�� �迭 ���� ����
	std::vector<Sprite*> _spriteList;
	// ���� ��������Ʈ �迭 ���, Ÿ�ϼ� �迭�� �����.
	std::vector<std::vector<TileCell*>> _tileArray;
	//Sprite* _spriteArray[mapHeight][mapWidth];
public:
	Map(std::wstring name);
	~Map();
public:
	//void Init(IDirect3DDevice9* device3d, ID3DXSprite* spriteDX);
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();

	void Update(float deltaTime);
	void Render();
	void Release();
	//void Reset(IDirect3DDevice9* device3d, ID3DXSprite* spriteDX);
	void Reset();

	Point GetPosition(int tileX, int tileY);
	// Component
	void setTileComponent(TilePoint tilePosition, Component* component);
	void ResetTileComponent(TilePoint tilePosition, Component* component);

	// Map Info
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	void CreateTiles(std::wstring scriptFilename);
	void CreateMazeTiles(std::wstring scriptFilename);
	bool IsConnectedCell(int tileX, int tileY);

	//
	void SetViewer(Component* component);
	void UpdateViewer(float deltaTime);
	bool CanMoveTile(TilePoint tilePosition);
	TileCell* GetTileCell(TilePoint tilePosition);
	//
	Component* FindComponentInRange(Component* finder, int range, std::vector<eComponentType> findTypeList);
	//
	std::vector<Component*> GetTileCollisionList(TilePoint newTilePosition);

	// 
	Component* FindItemInTile(TilePoint tilePosition);
	std::vector<Component*> GetTileComponentList(TilePoint tilePosition);
	void ResetPathfinding();

	TileCell* FindTileCellByMousePosition(int x, int y);
	//
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
};