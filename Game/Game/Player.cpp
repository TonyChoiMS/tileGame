#include "Player.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "InputSystem.h"
#include "MoveState.h"
#include "IdleState.h"

Player::Player(std::wstring name)
	: Character(name)
{
	_type = eComponentType::CT_PLAYER;
	_moveTime = 0.05f;
}
Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	if (false == _isLive)
	{
		return;
	}
	//if (false == _state->_isMoving)
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		if (NULL != map)
		{
			TilePoint newTilePosition = _tilePosition;
			//_currentDirection = rand() % 4;
			// 키입력에 따라서 _currentDirection 변화
			_nextDirection = eDirection::NONE;
			if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
			{
				_nextDirection = eDirection::LEFT;
			}
			if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
			{
				_nextDirection = eDirection::RIGHT;
			}
			if (InputSystem::GetInstance()->IsKeyDown(VK_UP))
			{
				_nextDirection = eDirection::UP;
			}
			if (InputSystem::GetInstance()->IsKeyDown(VK_DOWN))
			{
				_nextDirection = eDirection::DOWN;
			}
			if (InputSystem::GetInstance()->IsKeyDown(VK_SPACE))
			{
				EatItem();
			}
			//MoveStart(currentDirection)
			if (eDirection::NONE != _nextDirection)
			{
				_state->ChangeState(eStateType::ST_MOVE);
			}
			//_state->Start();
			/*
			switch (currentDirection)
			{
			case eDirection::LEFT:
			newTilePosition.x--;
			break;
			case eDirection::RIGHT:
			newTilePosition.x++;
			break;
			case eDirection::UP:
			newTilePosition.y--;
			break;
			case eDirection::DOWN:
			newTilePosition.y++;
			break;
			}
			if (eDirection::NONE != currentDirection)
			{
			_currentDirection = currentDirection;
			}
			if (newTilePosition.x != _tilePosition.x ||
			newTilePosition.y != _tilePosition.y)
			{
			if (0 <= newTilePosition.x && newTilePosition.x < map->GetWidth() &&
			0 <= newTilePosition.y && newTilePosition.y < map->GetHeight())
			{
			//_currentDirection = currentDirection;
			map->ResetTileComponent(_tilePosition, this);
			_tilePosition = newTilePosition;
			map->setTileComponent(_tilePosition, this);


			_isMoving = true;
			}
			}
			*/
		}
	}
	/*
	else
	{
	//if (_moveTime <= _state->_movingDuration)
	if (_moveTime <= _state->GetMovingDuration())
	{
	//_state->_movingDuration = 0.0f;
	//_state->_isMoving = false;
	//_state->SetMoving(false);
	_state->Stop();
	}
	else
	{
	//_state->_movingDuration += deltaTime;
	_state->UpdateMove(deltaTime);
	}
	}
	*/
}

std::vector<Component*> Player::Collision(std::vector<Component*> collisionList)
{
	std::vector<Component*> filterList;
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (eComponentType::CT_MONSTER == collisionList[i]->GetType())
		{

			//찾은 타겟을 넣어준다.
			filterList.push_back(collisionList[i]);
		}
	}
	return filterList;
}

/*
void Player::ReceiveMsg(const sMessageParam& param)
{
if (L"RecoveryHP" == param.message)
{
_hp += param.recoveryHP;
}
}
*/
