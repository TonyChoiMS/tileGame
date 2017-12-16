#include "Map.h"
#include "GameSystem.h"

extern int gStartX;
extern int gStartY;

Map::Map(std::wstring name)
{

}

Map::~Map()
{

}

void Map::Init(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{

	_width = 128;
	_height = 128;

	// 타일맵 인덱스 구성
	int testTileMapIndex[mapHeight][mapWidth];
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			testTileMapIndex[y][x] = rand() % 4;
		}
	}

	// 타일맵 인덱스를 이용해서 스프라이트 리스트를 구성
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			int spriteIndex = testTileMapIndex[y][x];
			Sprite* sprite = new Sprite(device3d, spriteDX);
			switch (spriteIndex)
			{
			case 0:
				sprite->Init(L"character_sprite.png", L"player_left.json");
				break;
			case 1:
				sprite->Init(L"character_sprite.png", L"player_right.json");
				break;
			case 2:
				sprite->Init(L"character_sprite.png", L"player_down.json");
				break;
			case 3:
				sprite->Init(L"character_sprite.png", L"player_up.json");
				break;
			}
			_spriteArray[y][x] = sprite;
		}
	}

	int tileSize = 32;
	//_renderWidth = clientWidth / tileSize + 1;
	//_renderHeight = clientHeight / tileSize + 1;
	_renderWidth = GameSystem::GetInstance()->GetClientWidth() / tileSize + 1;
	_renderHeight = GameSystem::GetInstance()->GetClientHeight() / tileSize + 1;

}
void Map::Deinit()
{

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			delete _spriteArray[y][x];
		}
	}

}

void Map::Update(int deltaTime)
{

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_spriteArray[y][x]->Update(deltaTime);
		}
	}

}

void Map::Render()
{

	int startTileX = gStartX;
	int startTileY = gStartY;
	int endTileX = startTileX + _renderWidth;
	int endTileY = startTileY + _renderHeight;

	if (_width < endTileX)
	{
		endTileX = _width;
	}
	if (_height < endTileY)
	{
		endTileY = _height;
	}

	float posX = 0.0f;
	float posY = 0.0f;
	int tileSize = 32;

	for (int y = startTileY; y < endTileY; y++)
	{
		if (0 <= y)
		{
			for (int x = startTileX; x < endTileX; x++)
			{
				if (0 <= x)
				{
					_spriteArray[y][x]->SetPosition(posX, posY);
					_spriteArray[y][x]->Render();
				}
				posX += tileSize;
			}
		}
		posX = 0.0f;
		posY += tileSize;
	}

}

void Map::Release()
{

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_spriteArray[y][x]->Release();
		}
	}

}

void Map::Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_spriteArray[y][x]->Reset(device3d, spriteDX);
		}
	}

}