#include "TileCell.h"
#include "Monster.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "MoveState.h"
#include "IdleState.h"
Monster::Monster(std::wstring name)
	: Character(name)
{
	_type = eComponentType::CT_MONSTER;
}
Monster::~Monster()
{

}

void Monster::UpdateAI(float deltaTime)
{
	if (false == _isLive)
	{
		return;
	}
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		if (NULL != map)
		{
			//NPC와 Player를 따라가는 간단한 AI (길찾기 기초버전)

			// 탐색 - 일정 범위 내에 적이 있는가?
			std::vector<eComponentType>findTypeList;
			findTypeList.push_back(eComponentType::CT_NPC);
			findTypeList.push_back(eComponentType::CT_PLAYER);
			Component* findComponent = map->FindComponentInRange(this, 4, findTypeList);
			/*
			int rangeMinX = _tilePosition.x - 4;
			int rangeMaxX = _tilePosition.x + 4;
			int rangeMinY = _tilePosition.y - 4;
			int rangeMaxY = _tilePosition.y + 4;

			if (rangeMinX < 0)
			{
			rangeMinX = 0;
			}
			if (map->GetWidth() <= rangeMaxX)
			{
			rangeMaxX = map->GetWidth() - 1;
			}
			if (rangeMinY < 0)
			{
			rangeMinY = 0;
			}
			if (map->GetHeight() <= rangeMaxY)
			{
			rangeMaxY = map->GetHeight() - 1;
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
			TileCell* tileCell = map->GetTileCell(tilePosition);
			std::list<Component*> collsionList = tileCell->GetCollisionList();
			if (0 < collsionList.size())
			{
			for (std::list<Component*>::iterator it = collsionList.begin();
			it != collsionList.end(); it++)
			{
			Component* component = (*it);
			if (eComponentType::CT_NPC == component->GetType() ||
			eComponentType::CT_PLAYER == component->GetType())
			{
			findComponent = component;
			break;
			}
			}
			}
			}
			}
			*/
			// 적이 있으면 적 방향으로 이동
			_nextDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
				{
					_nextDirection = eDirection::UP;
				}
				else if (_tilePosition.y < targetPosition.y)
				{
					_nextDirection = eDirection::DOWN;
				}
				else if (_tilePosition.x < targetPosition.x)
				{
					_nextDirection = eDirection::RIGHT;
				}
				else if (targetPosition.x < _tilePosition.x)
				{
					_nextDirection = eDirection::LEFT;
				}
				else
				{
					_nextDirection = (eDirection)(rand() % 4);
				}
			}
			else
			{
				_nextDirection = (eDirection)(rand() % 4);
			}
			if (eDirection::NONE != _nextDirection)
			{
				//MoveStart(currentDirection);
				//_state->Start();
				_state->ChangeState(eStateType::ST_MOVE);

			}

			//eDirection currentDirection = (eDirection)(rand() % 4);
			//MoveStart(currentDirection);
		}
	}
	/*
	else
	{
	if (_moveTime <= _state->GetMovingDuration())
	{
	//_state->_movingDuration = 0.0f;
	//_state->_isMoving = false;
	_state->Stop();
	}
	else
	{
	//_state->_movingDuration += deltaTime;
	_state->Update(deltaTime);
	}
	}
	*/
}

std::vector<Component*> Monster::Collision(std::vector<Component*> collisionList)
{
	//타겟 리스트를 전달
	//_targetList.clear();
	std::vector<Component*> filterList;
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (eComponentType::CT_NPC == collisionList[i]->GetType() ||
			eComponentType::CT_PLAYER == collisionList[i]->GetType())
		{
			/*
			// 적이면 공격
			sMessageParam param;
			param.sender = this;
			param.receiver = collisionList[i];
			param.message = L"Attack";
			param.attackPoint = _attackPoint;
			ComponentSystem::GetInstance()->SendMsg(param);
			*/
			//찾은 타겟을 넣어준다.
			filterList.push_back(collisionList[i]);
		}
	}
	return filterList;

	/*
	if (0 < _targetList.size())
	{
	ChangeState(eStateType::ST_ATTACK);
	}
	else
	{
	ChangeState(eStateType::ST_IDLE);
	}
	*/
}