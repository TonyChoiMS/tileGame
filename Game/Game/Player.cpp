#include "Player.h"

#include "Map.h"
#include "ComponentSystem.h"
#include "InputSystem.h"

Player::Player(std::wstring name) : Character(name)
{
	_moveTime = 0.15f;
}

Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			TilePoint newTilePosition = _tilePosition;
			//_currentDirection = rand() % 4;
			// 키입력에 따라서 _currentDirection 변화
			int currentDirection = -1;
			if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
				currentDirection = 0;
			if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
				currentDirection = 1;
			if (InputSystem::GetInstance()->IsKeyDown(VK_UP))
				currentDirection = 2;
			if (InputSystem::GetInstance()->IsKeyDown(VK_DOWN))
				currentDirection = 3;
			
			switch (currentDirection)
			{
			case 0:	newTilePosition.x--; break;
			case 1:	newTilePosition.x++; break;
			case 2:	newTilePosition.y--; break;
			case 3:	newTilePosition.y++; break;
			}
			
			if (0 <= currentDirection)
				_currentDirection = currentDirection;

			if (newTilePosition.x != _tilePosition.x ||
				newTilePosition.y != _tilePosition.y)
			{
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