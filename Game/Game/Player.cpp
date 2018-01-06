#include "Player.h"

#include "Map.h"
#include "ComponentSystem.h"
#include "InputSystem.h"

Player::Player(std::wstring name) : Character(name)
{
	_moveTime = 0.15f;
	_type = eComponentType::CT_PLAYER;
}

Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	if (false == _isLive)
		return;

	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{

			// 키입력에 따라서 _currentDirection 변화
			eDirection currentDirection = eDirection::NONE;
			if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
				currentDirection = eDirection::LEFT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
				currentDirection = eDirection::RIGHT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_UP))
				currentDirection = eDirection::UP;
			if (InputSystem::GetInstance()->IsKeyDown(VK_DOWN))
				currentDirection = eDirection::DOWN;

			MoveStart(currentDirection);
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