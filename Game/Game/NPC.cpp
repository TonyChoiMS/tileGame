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
			_currentDirection = (eDirection)(rand() % 4);
			switch (_currentDirection)
			{
			case eDirection::LEFT:	newTilePosition.x--; break;
			case eDirection::RIGHT:	newTilePosition.x++; break;
			case eDirection::UP:	newTilePosition.y--; break;
			case eDirection::DOWN:	newTilePosition.y++; break;
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