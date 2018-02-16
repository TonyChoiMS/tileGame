#include "Player.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "InputSystem.h"
#include "MoveState.h"
#include "IdleState.h"

Player::Player(std::wstring name) : Character(name)
{
	_type = eComponentType::CT_PLAYER;
	_moveTime = 0.15f;
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
			// Ű�Է¿� ���� _currentDirection ��ȭ
			_nextDirection = eDirection::NONE;
			if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
				_nextDirection = eDirection::LEFT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
				_nextDirection = eDirection::RIGHT;
			if (InputSystem::GetInstance()->IsKeyDown(VK_UP))
				_nextDirection = eDirection::UP;
			if (InputSystem::GetInstance()->IsKeyDown(VK_DOWN))
				_nextDirection = eDirection::DOWN;
			if (InputSystem::GetInstance()->IsKeyDown(VK_SPACE))
				EatItem();

			if (eDirection::NONE != _nextDirection)
				_state->ChangeState(eStateType::ST_MOVE);

		}
	}
}

std::vector<Component*> Player::Collision(std::vector<Component*> collisionList)
{
	std::vector<Component*> filterList;
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (eComponentType::CT_MONSTER == collisionList[i]->GetType())
		{
			//ã�� Ÿ���� �־��ش�.
			filterList.push_back(collisionList[i]);
		}
	}
	return filterList;
}

