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
	// Sprite List ����
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
	// Ÿ�ϸ� �ε��� ���� -> ��Ʈ��Ʈ��	�������� ��������Ʈ�� ����
	_tileArray.clear();			// 1. ��������Ʈ �迭�� �ʱ�ȭ(����)

	// 1��
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// ��ū���� ������, ���ӵ����ͷ� ��ȯ
		char record[1000];
		char* token;
		char* context = NULL;

		// ù° ������ �� ũ�� ����
		strcpy_s(record, recordList[0].c_str());
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
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList;							// 2. ���� �迭 ����
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray ����
				int spriteIndex = atoi(token);						// ��ũ��Ʈ�� ��ü�� �κ�
				Sprite* sprite = _spriteList[spriteIndex];			// �߿��� �ٽɺκ�	

				TileCell* tileCell = new TileCell();
				tileCell->Init();

				// Ÿ�� ������Ʈ�� ����
				WCHAR name[256];
				wsprintf(name, L"map_layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);							// 3. ���� �迭�� ���� ä��.
				token = strtok(NULL, ",");							// strtok 1��° �Ķ���Ϳ� NULL�� ���� ���, ���� ���� �ƴ� ���������� ��ĭ�̵�(���� ĭ)
																	//_spriteArray[y][x] = sprite;
			}
			_tileArray.push_back(rowList);		// 4. ���θ� ����Ʈ�� �־ ���� �ϼ�
			line++;									// �� ������ ���� ����
		}
	}

	// 2��
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"02.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		// ��ū���� ������, ���ӵ����ͷ� ��ȯ
		char record[1000];
		char* token;

		// ù° ������ �� ũ�� ����
		strcpy_s(record, recordList[0].c_str());
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
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList = _tileArray[y];			// 2. ���� �迭 ����
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray ����
				int spriteIndex = atoi(token);						// ��ũ��Ʈ�� ��ü�� �κ�
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];			// �߿��� �ٽɺκ�	
					TileCell* tileCell = rowList[x];

					// Ÿ�� ������Ʈ�� ����
					WCHAR name[256];
					wsprintf(name, L"map_layer02_%d_%d", line, x);
					TileObject* tileObject = new TileObject(name, sprite);
					tileObject->SetCanMove(false);
					tileCell->AddComponent(tileObject);
				}
				token = strtok(NULL, ",");							// strtok 1��° �Ķ���Ϳ� NULL�� ���� ���, ���� ���� �ƴ� ���������� ��ĭ�̵�(���� ĭ)
			}
			_tileArray.push_back(rowList);		// 4. ���θ� ����Ʈ�� �־ ���� �ϼ�
			line++;									// �� ������ ���� ����
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

	// ���� üũ (�� �ȿ� �ִ���)
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
			// NPC�� player�� ����
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
					// ������� Ÿ�԰˻�
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

	// ���� üũ (�� �ȿ� �ִ���)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
		return findArray;			// �� ���� �ۿ� ���� ��� �ƹ��͵� add���� �ʰ� push

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

