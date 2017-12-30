#include "ComponentSystem.h"
#include "Map.h"

#include "NPC.h"

NPC::NPC(std::wstring name) : Character(name)
{

}

NPC::~NPC()
{

}

void NPC::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			TilePoint newTilePosition = _tilePosition;
			_currentDirection = rand() % 4;
			switch (_currentDirection)
			{
			case 0:	newTilePosition.x--; break;
			case 1:	newTilePosition.x++; break;
			case 2:	newTilePosition.y--; break;
			case 3:	newTilePosition.y++; break;
			}
			if (0 <= newTilePosition.x && newTilePosition.x < map->GetWidth() &&
				0 <= newTilePosition.y && newTilePosition.y < map->GetHeight())
			{
				map->ResetTileComponent(_tilePosition, this);
				_tilePosition = newTilePosition;
				map->SetTileComponent(_tilePosition, this);

				_isMoving = true;
			}
		}
	}
	else
	{
		if (_moveTime <= _movingDuration)
		{
			_movingDuration = 0.0f;
			_isMoving = false;
		}
		else
		{
			_movingDuration += deltaTime;
		}
	}
}