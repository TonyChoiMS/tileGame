#include <string>

#include "Map.h"
#include "ResourceManager.h"
#include "GameSystem.h"

Map::Map(std::wstring name)
{
	_startX = 0;
	_startY = 0;
}

Map::~Map()
{

}

void Map::Init(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_width = mapWidth;
	_height = mapHeight;

	// Sprite List ����
	{
		int srcX = 0;
		int srcY = 0;
		int spriteSize = 32;
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				Sprite* sprite = new Sprite(device3d, spriteDX);
				sprite->Init(L"MapSprite.png", srcX, srcY, spriteSize, spriteSize, 1.0f);
				_spriteList.push_back(sprite);

				srcX += spriteSize;
			}
			srcX = 0;
			srcY += spriteSize;
		}
	}

	// Ÿ�ϸ� �ε��� ����
	
	/*
	int index = 0;
	int testTileMapIndex[mapHeight][mapWidth];
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			testTileMapIndex[y][x] = index;
			index++;
		}
	}
	*/
	// Ÿ�ϸ� �ε��� ���� -> ��Ʈ��Ʈ��	�������� ��������Ʈ�� ����
	_spriteArray.clear();			// 1. ��������Ʈ �迭�� �ʱ�ȭ(����)
	{
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(L"MapData.csv");

		// ��ū���� ������, ���ӵ����ͷ� ��ȯ
		char record[1000];
		char* token;
		char* context = NULL;

		// ù° ������ �� ũ�� ����
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");		// ù��° ĭ�� ��ŵ (mapsize)
		token = strtok(NULL, ",");			// �ι�° ĭ ���� ���� ũ�� : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// ����° ĭ ���� ���� ũ�� : 10
		_height = atoi(token);

		// �ι�° ������ ��ŵ
		// ����° ���κ��Ͱ� ���� �� �ε��� ������
		int line = 2;
		for (int y = 0; y < _height; y++) 
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<Sprite*> rowList;							// 2. ���� �迭 ����
			for (int x = 0; x < _width; x++) 
			{
				// _spriteArray ����
				int spriteIndex = atoi(token);						// ��ũ��Ʈ�� ��ü�� �κ�
				Sprite* sprite = _spriteList[spriteIndex];			// �߿��� �ٽɺκ�	
				rowList.push_back(sprite);							// 3. ���� �迭�� ���� ä��.
				token = strtok(NULL, ",");							// strtok 1��° �Ķ���Ϳ� NULL�� ���� ���, ���� ���� �ƴ� ���������� ��ĭ�̵�(���� ĭ)
																	//_spriteArray[y][x] = sprite;
			}
			_spriteArray.push_back(rowList);		// 4. ���θ� ����Ʈ�� �־ ���� �ϼ�
			line++;									// �� ������ ���� ����
		}
	}
	
	// Ÿ�ϸ� �ε����� �̿��ؼ� �� ����
	/*
	for (int y = 0; y < _height; y++)
	{
		std::vector<Sprite*> rowList;		// 2. ���� �迭 ����
		for (int x = 0; x < _width; x++)
		{
			int spriteIndex = testTileMapIndex[y][x];
//			Sprite* sprite = new Sprite(device3d, spriteDX);
			Sprite* sprite = _spriteList[spriteIndex];			// �߿��� �ٽɺκ�	
			//_spriteArray[y][x] = sprite;
		}
		_spriteArray.push_back(rowList);		// 4. ���θ� ����Ʈ�� �־ ���� �ϼ�
	}
	*/

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

	int startTileX = _startX;
	int startTileY = _startY;
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

void Map::MoveLeft()
{
	_startX--;
}

void Map::MoveRight()
{
	_startX++;
}

void Map::MoveUp()
{
	_startY--;
}

void Map::MoveDown()
{
	_startY++;
}