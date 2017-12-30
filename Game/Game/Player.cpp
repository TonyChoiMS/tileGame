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
			// Ű�Է¿� ���� _currentDirection ��ȭ
			eDirection currentDirection = eDirection::NONE;
			if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
				currentDirection = eDirection::LEFT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
				currentDirection = eDirection::RIGHT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_UP))
				currentDirection = eDirection::UP;
			if (InputSystem::GetInstance()->IsKeyDown(VK_DOWN))
				currentDirection = eDirection::DOWN;
			
			switch (currentDirection)
			{
			case eDirection::LEFT:	newTilePosition.x--; break;
			case eDirection::RIGHT:	newTilePosition.x++; break;
			case eDirection::UP:	newTilePosition.y--; break;
			case eDirection::DOWN:	newTilePosition.y++; break;
			}
			
			if (eDirection::NONE != currentDirection)
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

					// ĳ���Ͱ� �̵��ϸ� �ʵ� �̵������ش�.
					/*
					switch (currentDirection)
					{
					case 0:
						map->MoveLeft();
						break;
					case 1:
						map->MoveRight();
						break;
					case 2:
						map->MoveUp();
						break;
					case 3:
						map->MoveDown();
						break;
					}
					*/
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