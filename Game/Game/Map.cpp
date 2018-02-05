#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <string>

#include "GameSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "Map.h"

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

	_tileSize = 0;
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
	// 타일맵 인덱스 구성 -> 스트립트를	바탕으로 스프라이트를 생성
	_tileArray.clear();			// 1. 스프라이트 배열을 초기화(비우기)

	// 1층
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;
		char* context = NULL;

		// 첫째 라인은 맵 크기 정보
		strcpy_s(record, recordList[0].c_str());
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
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList;							// 2. 가로 배열 생성
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);						// 스크립트로 대체된 부분
				Sprite* sprite = _spriteList[spriteIndex];			// 중요한 핵심부분	

				TileCell* tileCell = new TileCell();
				tileCell->Init();

				// 타일 오브젝트를 생성
				WCHAR name[256];
				wsprintf(name, L"map_layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);							// 3. 가로 배열에 내용 채움.
				token = strtok(NULL, ",");							// strtok 1번째 파라미터에 NULL을 넣을 경우, 다음 줄이 아닌 오른쪽으로 한칸이동(다음 칸)
																	//_spriteArray[y][x] = sprite;
			}
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;									// 다 읽으면 라인 증가
		}
	}

	// 2층
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"02.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// 토큰으로 나눠서, 게임데이터로 변환
		char record[1000];
		char* token;

		// 첫째 라인은 맵 크기 정보
		strcpy_s(record, recordList[0].c_str());
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
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList = _tileArray[y];			// 2. 가로 배열 생성
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray 구성
				int spriteIndex = atoi(token);						// 스크립트로 대체된 부분
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];			// 중요한 핵심부분	
					TileCell* tileCell = rowList[x];

					// 타일 오브젝트를 생성
					WCHAR name[256];
					wsprintf(name, L"map_layer02_%d_%d", line, x);
					TileObject* tileObject = new TileObject(name, sprite);
					tileObject->SetCanMove(false);
					tileCell->AddComponent(tileObject);
				}
				token = strtok(NULL, ",");							// strtok 1번째 파라미터에 NULL을 넣을 경우, 다음 줄이 아닌 오른쪽으로 한칸이동(다음 칸)
			}
			_tileArray.push_back(rowList);		// 4. 가로를 리스트에 넣어서 세로 완성
			line++;									// 다 읽으면 라인 증가
		}
	}

	_tileSize = 32;
	_renderWidth = GameSystem::GetInstance()->GetClientWidth() / _tileSize + 1;
	_renderHeight = GameSystem::GetInstance()->GetClientHeight() / _tileSize + 1;

	_startX = _startY = 0;
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
			_tileArray[y][x]->Update(deltaTime);
		}
	}

	UpdateViewer(deltaTime);
}

void Map::Render()
{
	int midTileCountX = _renderWidth / 2;
	int midTileCountY = _renderHeight / 2;

	int startTileX = _viewer->GetTilePosition().x - midTileCountX - 1;
	int startTileY = _viewer->GetTilePosition().y - midTileCountY - 1;
	int endTileX = startTileX + _renderWidth + 1;
	int endTileY = startTileY + _renderHeight + 1;

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
			_tileArray[y][x]->Reset();
		}
	}
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

void Map::SetTileComponent(TilePoint tilePosition, Component* component)
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

std::vector<Component*> Map::GetTileCollisionList(TilePoint tilePosition)
{
	std::vector<Component*> collisiontArray;

	// 범위 체크 (맵 안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
		return collisiontArray;

	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetCollisionList();
	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		collisiontArray.push_back((*it));
	}
	return collisiontArray;
}

Component* Map::FindComponentInRange(Component* finder, int range, std::vector<eComponentType> findTypeList)
{
	int rangeMinX = finder->GetTilePosition().x - range;
	int rangeMaxX = finder->GetTilePosition().x + range;
	int rangeMinY = finder->GetTilePosition().y - range;
	int rangeMaxY = finder->GetTilePosition().y + range;

	if (rangeMinX < 0)
	{
		rangeMinX = 0;
	}
	if (GetWidth() <= rangeMaxX)
	{
		rangeMaxX = GetWidth() - 1;
	}
	if (rangeMinY < 0)
	{
		rangeMinY = 0;
	}
	if (GetHeight() <= rangeMaxY)
	{
		rangeMaxY = GetHeight() - 1;
	}

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
					// 살았으면 타입검사
					if (component->IsLive())
					{
						for (int i = 0; i < findTypeList.size(); i++)
						{
							if (findTypeList[i] = component->GetType())
								return component;
						}
					}
				}
			}
		}
	}
	return NULL;
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

TileCell* Map::GetTileCell(TilePoint tilePosition)
{
	return _tileArray[tilePosition.y][tilePosition.x];
}

std::vector<Component*> Map::GetTileComponentList(TilePoint tilePosition)
{
	std::vector<Component*> findArray;

	// 범위 체크 (맵 안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
		return findArray;			// 맵 범위 밖에 있을 경우 아무것도 add하지 않고 push

	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetComponentList();
	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		findArray.push_back((*it));
	}
	return findArray;
}

Component* Map::FindItemInTile(TilePoint tilePosition)
{
	std::vector<Component*> componentList = GetTileComponentList(tilePosition);
	for (int i = 0; i < componentList.size(); i++)
	{
		Component* component = componentList[i];
		if (eComponentType::CT_ITEM == component->GetType())
			return component;
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

