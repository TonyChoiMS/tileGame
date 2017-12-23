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

	// Sprite List 구성
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

	// 타일맵 인덱스 구성
	
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
	// 타일맵 인덱스 구성 -> 스트립트를	바탕으로 스프라이트를 생성
	_spriteArray.clear();			// 1. 스프라이트 배열을 초기화(비우기)
	{
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(L"MapData.csv");

		// 토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		char* context = NULL;

		// 첫째 라인은 맵 크기 정보
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번째 칸은 스킵 (mapsize)
		token = strtok(NULL, ",");			// 두번째 칸 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번째 칸 실제 세로 크기 : 10
		_height = atoi(token);

		// 두번째 라인은 스킵
		// 세번째 라인부터가 실제 맵 인덱스 데이터
		int line = 2;
		for (int y = 0; y < _height; y++) 
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<Sprite*> rowList;							// 2. 가로 배열 생성
			for (int x = 0; x < _width; x++) 
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);						// 스크립트로 대체된 부분
				Sprite* sprite = _spriteList[spriteIndex];			// 중요한 핵심부분	
				rowList.push_back(sprite);							// 3. 가로 배열에 내용 채움.
				token = strtok(NULL, ",");							// strtok 1번째 파라미터에 NULL을 넣을 경우, 다음 줄이 아닌 오른쪽으로 한칸이동(다음 칸)
																	//_spriteArray[y][x] = sprite;
			}
			_spriteArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;									// 다 읽으면 라인 증가
		}
	}
	
	// 타일맵 인덱스를 이용해서 맵 구성
	/*
	for (int y = 0; y < _height; y++)
	{
		std::vector<Sprite*> rowList;		// 2. 가로 배열 생성
		for (int x = 0; x < _width; x++)
		{
			int spriteIndex = testTileMapIndex[y][x];
//			Sprite* sprite = new Sprite(device3d, spriteDX);
			Sprite* sprite = _spriteList[spriteIndex];			// 중요한 핵심부분	
			//_spriteArray[y][x] = sprite;
		}
		_spriteArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
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