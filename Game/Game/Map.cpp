#include <string>

#include "Map.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "GameSystem.h"
#include "ResourceManager.h"



Map::Map(std::wstring name) : Component(name)
{
}

Map::~Map()
{
}

void Map::Init(std::wstring textureFilename, std::wstring scriptFilename)
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
				Sprite* sprite = new Sprite();
				sprite->Init(textureFilename, srcX, srcY, spriteSize, spriteSize, 1.0f);
				_spriteList.push_back(sprite);
				srcX += spriteSize;
			}
			srcX = 0;
			srcY += spriteSize;
		}
	}
	// 타일 맵 인덱스 구성 -> 스크립트를 바탕으로 스프라이트를 생성
	_tileArray.clear();

	//CreateTiles(scriptFilename);
	CreateMazeTiles(scriptFilename);

	_tileSize = 32;
	_renderMapWidth = GameSystem::GetInstance()->GetClientWidth() / _tileSize + 1;
	_renderMapHeight = GameSystem::GetInstance()->GetClientHeight() / _tileSize + 1;

	_startX = 0;
	_startY = 0;
}

void Map::Deinit()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			// _sprtieArray -> _tileArray 로 변경

			_tileArray[y][x]->Update(deltaTime);
		}
	}
	UpdateViewer(deltaTime);
}

void Map::Render()
{
	int midTileCountX = _renderMapWidth / 2;
	int midTileCountY = _renderMapHeight / 2;

	int startTileX = _viewer->GetTilePosition().x - midTileCountX - 1;
	int startTileY = _viewer->GetTilePosition().y - midTileCountY - 1;

	int endTileX = startTileX + _renderMapWidth + 1;
	int endTileY = startTileY + _renderMapHeight + 1;

	if (_width < endTileX)
		endTileX = _width;
	if (_height < endTileY)
		endTileY = _height;

	Point renderPosition;
	renderPosition.x = renderPosition.y = 0.0f;

	for (int y = startTileY; y < endTileY; y++)
	{
		if (0 <= y)
		{
			for (int x = startTileX; x < endTileX; x++)
			{
				if (0 <= x)
				{
					// _sprtieArray -> _tileArray 로 변경
					_tileArray[y][x]->SetPosition(renderPosition);
					_tileArray[y][x]->Render();
				}
				renderPosition.x += _tileSize;

			}
		}
		renderPosition.x = 0.0f;
		renderPosition.y += _tileSize;
	}

}

void Map::Release()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			// _sprtieArray -> _tileArray 로 변경
			_tileArray[y][x]->Release();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			// _sprtieArray -> _tileArray 로 변경
			_tileArray[y][x]->Reset();
		}
	}
}

void Map::CreateTiles(std::wstring scriptFilename)
{
	// 1 층 Layer01
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		// 1. 리소스 매니저를 이용해서 레코드 리스트를 얻는다.
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 2.  토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		//  첫 째 라인은 맵 크기 정보
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸을 스킵 : mapSize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 가로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이터
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList;
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);	// <- 스크립트로 대체된 부분
				Sprite* sprite = _spriteList[spriteIndex];		// <- 중요

				TileCell* tileCell = new TileCell();
				tileCell->Init(x, y);

				// 타일 오브젝트 생성
				WCHAR name[256];
				wsprintf(name, L"map_Layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite, x, y);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);
				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;
		}
	}

	// 2 층 Layer02
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"02.csv");
		// 1. 리소스 매니저를 이용해서 레코드 리스트를 얻는다.
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 2.  토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		//  첫 째 라인은 맵 크기 정보
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸을 스킵 : mapSize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 가로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이터
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList = _tileArray[y];
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);	// <- 스크립트로 대체된 부분
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];

					TileCell* tileCell = rowList[x];

					// 타일 오브젝트 생성
					WCHAR name[256];
					wsprintf(name, L"map_Layer02_%d_%d", line, x);
					TileObject* tileObject = new TileObject(name, sprite, x, y);
					tileObject->SetCanMove(false);
					tileCell->AddComponent(tileObject);

				}

				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;
		}
	}
	// 타일 맵 인덱스를 이용해서 맵 구성
	/*
	//Sprite* testTileMapSprite[mapHeight][mapWidth];
	_spriteArray.clear();							// 1. 기존의 스프라이트 배열을 초기화 (비우고)

	for (int y = 0; y < _height; y++)
	{
	std::vector<Sprite*> rowList;				// 2. 가로열 배열 생성
	for (int x = 0; x < _width; x++)
	{
	int spriteIndex = testTileMapIndex[y][x];
	//Sprite* sprite = new Sprite(device3d, spriteDX);
	Sprite* sprite = _spriteList[spriteIndex];			// <- 중요
	rowList.push_back(sprite);				// 3. 가로열 배열에 내용 채움
	// todo : 스프라이트 인덱스에 맞는 스크립트 로드

	//_spriteArray[y][x] = sprite;
	}
	_spriteArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
	}
	*/
	/*
	switch (spriteIndex)
	{
	case 0:
	sprite->Init(L"character_sprite.png", L"player_up.json");
	break;
	case 1:
	sprite->Init(L"character_sprite.png", L"player_down.json");
	break;
	case 2:
	sprite->Init(L"character_sprite.png", L"player_right.json");
	break;
	case 3:
	sprite->Init(L"character_sprite.png", L"player_left.json");
	break;

	}
	*/
}

void Map::CreateMazeTiles(std::wstring scriptFilename)
{
	// 1 층 Layer01
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		// 1. 리소스 매니저를 이용해서 레코드 리스트를 얻는다.
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 2.  토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		//  첫 째 라인은 맵 크기 정보
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸을 스킵 : mapSize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 가로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이터
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			//std::vector<Sprite*> rowList;
			std::vector<TileCell*> rowList;
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);	// <- 스크립트로 대체된 부분
				Sprite* sprite = _spriteList[spriteIndex];	// <- 중요

				TileCell* tileCell = new TileCell();
				tileCell->Init(x, y);

				// 타일 오브젝트 생성
				WCHAR name[256];
				wsprintf(name, L"map_Layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite, x, y);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);
				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;
		}

	}
	
	/*
	// 2 층 Layer02
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"02.csv");
		// 1. 리소스 매니저를 이용해서 레코드 리스트를 얻는다.
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 2.  토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		//  첫 째 라인은 맵 크기 정보
		strcpy(record, recordList[0].c_str());
		token = strtok(record, ",");				// 첫번 째 칸을 스킵 : mapSize
		token = strtok(NULL, ",");					// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");					// 세번 째 실제 가로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이터
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy(record, recordList[line].c_str());
			token = strtok(record, ",");

			//std::vector<Sprite*> rowList;
			std::vector<TileCell*> rowList = _tileArray[y];
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);				// <- 스크립트로 대체된 부분
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];

					TileCell* tileCell = rowList[x];
					//tileCell->Init();

					WCHAR name[256];
					wsprintf(name, L"map_Layer02_%d_%d", line, x);
					TileObject* tileObject = new TileObject(name, sprite, x, y);
					tileObject->SetCanMove(false);
					tileCell->AddComponent(tileObject);

				}

				token = strtok(NULL, ",");
			}
			//_spriteArray.puch_bakc(rowList);
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;
		}

	}
	*/
	

	// 기준이 되는 블럭을 심는다.
	for (int y = 0; y < _height; y++) 
	{
		if (0 == (y % 2))
		{
			std::vector<TileCell*> rowList = _tileArray[y];
			for (int x = 0; x < _width; x++)
			{
				if (0 == (x % 2))
				{
					int spriteIndex = 139;
					if (0 <= spriteIndex)
					{
						Sprite* sprite = _spriteList[spriteIndex];

						TileCell* tileCell = rowList[x];
						//tileCell->Init();

						// 타일 오브젝트 생성
						WCHAR name[256];
						wsprintf(name, L"map_Layer02_%d_%d", y, x);
						TileObject* tileObject = new TileObject(name, sprite, x, y);
						tileObject->SetCanMove(false);
						tileCell->AddComponent(tileObject);

					}
				}
			}
			//_tileArray.push_back(rowList);
		}
	}

	// 기준이 되는 블럭을 연결해 나간다.
	for (int y = 0; y < _height; y++)
	{
		//std::vector<TileCell*> rowList = _tileArray[y];
		for (int x = 0; x < _width; x++)
		{
			TilePoint pos;
			pos.x = x;
			pos.y = y;
			if (false == GetTileCell(pos)->CanMove())
			{
				if (false == IsConnectedCell(x, y))
				{
					// 한 방향을 결정해서, 다음 블럭을 만날 때 까지(연결될 때 까지) 이어준다.
					eDirection direction = (eDirection)(rand() % (int)eDirection::NONE);

					// 방향에 따른 다음 타일 위치를 구해서, 그 위치에 블럭 생성
					int newTileX = x;
					int newTileY = y;
					while (false == IsConnectedCell(newTileX, newTileY))
					{
						switch (direction)
						{
						case eDirection::LEFT:
							newTileX--;
							break;
						case eDirection::RIGHT:
							newTileX++;
							break;
						case eDirection::UP:
							newTileY--;
							break;
						case eDirection::DOWN:
							newTileY++;
							break;
						}
						if (0 <= newTileX && newTileX < _width &&
							0 <= newTileY && newTileY < _height)
						{
							int spriteIndex = 139;
							if (0 <= spriteIndex)
							{
								Sprite* sprite = _spriteList[spriteIndex];

								std::vector<TileCell*> rowList = _tileArray[newTileY];
								TileCell* tileCell = rowList[newTileX];

								//TileCell* tileCell = rowList[x];
								//tileCell->Init();

								// 타일 오브젝트 생성
								WCHAR name[256];
								wsprintf(name, L"map_Layer02_%d_%d", y, x);
								TileObject* tileObject = new TileObject(name, sprite, x, y);
								tileObject->SetCanMove(false);
								tileCell->AddComponent(tileObject);

							}
						}
					}
				}
			}
		}
	}
}

bool Map::IsConnectedCell(int tileX, int tileY)
{
	// 주변에 하나라도 붙은 블럭이 있으면 연결된 블럭이다.
	for (int direction = 0; direction < (int)eDirection::NONE; direction++)
	{
		int searchTileX = tileX;
		int searchTileY = tileY;
		switch(direction)
		{
		case eDirection::LEFT: searchTileX--; break;
		case eDirection::RIGHT: searchTileX++; break;
		case eDirection::UP: searchTileY--; break;
		case eDirection::DOWN: searchTileY++; break;
		}
		if (0 <= searchTileX && searchTileX < _width &&
			0 <= searchTileY && searchTileY < _height)
		{
			TilePoint position;
			position.x = searchTileX;
			position.y = searchTileY;
			if (false == GetTileCell(position)->CanMove())
				return true;
		}
	}
	return false;
}

void Map::SetViewer(Component* component)
{
	_viewer = component;
	_prevViewTilePosition = _viewer->GetTilePosition();
}

void Map::UpdateViewer(float deltaTime)
{
	if (NULL == _viewer)
		return;

	if (_prevViewTilePosition.x != _viewer->GetTilePosition().x ||
		_prevViewTilePosition.y != _viewer->GetTilePosition().y)
	{
		if (_prevViewTilePosition.x < _viewer->GetTilePosition().x)
			MoveRight();
		if (_viewer->GetTilePosition().x < _prevViewTilePosition.x)
			MoveLeft();
		if (_prevViewTilePosition.y < _viewer->GetTilePosition().y)
			MoveDown();
		if (_viewer->GetTilePosition().y < _prevViewTilePosition.y)
			MoveUp();

		_prevViewTilePosition = _viewer->GetTilePosition();
	}
}

void Map::setTileComponent(TilePoint tilePosition, Component* component)
{
	_tileArray[tilePosition.y][tilePosition.x]->AddComponent(component);
}

void Map::ResetTileComponent(TilePoint tilePosition, Component* component)
{
	_tileArray[tilePosition.y][tilePosition.x]->RemoveComponent(component);

}

Point Map::GetPosition(int tileX, int tileY)
{
	Point point;
	point.x = tileX * _tileSize;
	point.y = tileY * _tileSize;

	return point;

}

TileCell* Map::GetTileCell(TilePoint tilePosition)
{
	return _tileArray[tilePosition.y][tilePosition.x];
}

Component* Map::FindComponentInRange(Component* finder, int range, std::vector<eComponentType> findTypeList)
{
	int rangeMinX = finder->GetTilePosition().x - range;
	int rangeMaxX = finder->GetTilePosition().x + range;
	int rangeMinY = finder->GetTilePosition().y - range;
	int rangeMaxY = finder->GetTilePosition().y + range;

	if (rangeMinX < 0)
		rangeMinX = 0;
	if (GetWidth() <= rangeMaxX)
		rangeMaxX = GetWidth() - 1;
	if (rangeMinY < 0)
		rangeMinY = 0;
	if (GetHeight() <= rangeMaxY)
		rangeMaxY = GetHeight() - 1;

	Component* findComponent = NULL;
	for (int y = rangeMinY; y < rangeMaxY; y++)
	{
		for (int x = rangeMinX; x < rangeMaxX; x++)
		{
			// NPC와 player를 검출
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;
			TileCell* tileCell = GetTileCell(tilePosition);
			std::list<Component*> collsionList = tileCell->GetCollisionList();
			if (0 < collsionList.size())
			{
				for (std::list<Component*>::iterator it = collsionList.begin();
					it != collsionList.end(); it++)
				{
					Component* component = (*it);
					// 살았으면 타입 검사
					if (component->IsLive())
					{
						for (int i = 0; i < findTypeList.size(); i++)
						{
							if (findTypeList[i] == component->GetType())
							{
								return component;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

std::vector<Component*> Map::GetTileCollisionList(TilePoint tilePosition)
{
	std::vector<Component*> collisionArray;
	// 범위 체크 (맵안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
	{
		return collisionArray;
	}
	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetCollisionList();

	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		collisionArray.push_back((*it));
	}
	return collisionArray;
}

bool Map::CanMoveTile(TilePoint tilePosition)
{
	if (0 <= tilePosition.x && tilePosition.x < GetWidth() &&
		0 <= tilePosition.y && tilePosition.y < GetHeight())
	{
		return GetTileCell(tilePosition)->CanMove();
	}
	return false;
}

Component* Map::FindItemInTile(TilePoint tilePosition)
{
	std::vector<Component*> componentList = GetTileComponentList(tilePosition);
	for (int i = 0; i < componentList.size(); i++)
	{
		Component* component = componentList[i];
		if (eComponentType::CT_ITEM == component->GetType())
		{
			return component;
		}
	}
	return NULL;
}

std::vector<Component*> Map::GetTileComponentList(TilePoint tilePosition)
{
	std::vector<Component*> findArray;
	// 범위 체크 (맵안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
	{
		return findArray;
	}
	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetComponentList();

	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		findArray.push_back((*it));
	}
	return findArray;
}

void Map::ResetPathfinding()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;
			GetTileCell(tilePosition)->ResetPathfinding();
		}
	}
}

TileCell* Map::FindTileCellByMousePosition(int MouseX, int MouseY)
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	int minX = _viewer->GetTilePosition().x - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTilePosition().x + (midX / _tileSize) + 2;

	int minY = _viewer->GetTilePosition().y - (midY / _tileSize) - 2;
	int maxY = _viewer->GetTilePosition().y + (midY / _tileSize) + 2;

	if (minX < 0)
		minX = 0;
	if (_width < maxX)
		maxX = _width;
	if (minY < 0)
		minY = 0;
	if (_height < maxY)
		maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			RECT rect;
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;
			rect.left = GetTileCell(tilePosition)->GetPosition().x - _tileSize / 2;
			rect.right = GetTileCell(tilePosition)->GetPosition().x + _tileSize / 2;
			rect.top = GetTileCell(tilePosition)->GetPosition().y - _tileSize / 2;
			rect.bottom = GetTileCell(tilePosition)->GetPosition().y + _tileSize / 2;

			if (rect.left <= MouseX && MouseX <= rect.right &&
				rect.top <= MouseY && MouseY <= rect.bottom)
			{
				return GetTileCell(tilePosition);
			}
		}
	}
	return NULL;
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