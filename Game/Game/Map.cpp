#include <string>

#include "Map.h"
#include "ResourceManager.h"
#include "GameSystem.h"
#include "TileCell.h"
#include "TileObject.h"

Map::Map(std::wstring name) : Component(name)
{
	_startX = 0;
	_startY = 0;
}

Map::~Map()
{

}

void Map::Init(std::wstring textureFilename, std::wstring scriptFilename)
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

			std::vector<TileCell*> rowList = _tileArray[y];			// 2. ���� �迭 ����
			for (int x = 0; x < _width; x++)
			{
				// _spriteArray ����
				int spriteIndex = atoi(token);						// ��ũ��Ʈ�� ��ü�� �κ�
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];			// �߿��� �ٽɺκ�	
					TileCell* tileCell = rowList[x];
					//tileCell->Init();

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