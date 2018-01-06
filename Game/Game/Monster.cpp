#include "TileCell.h"
#include "Monster.h"
#include "Map.h"
#include "ComponentSystem.h"

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
		return;

	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			//NPC와 Player를 따라가는 간단한 AI (길찾기 기초버전)

			// 탐색 - 일정 범위 내에 적이 있는가?
			std::vector<eComponentType>findTypeList;
			findTypeList.push_back(eComponentType::CT_NPC);
			findTypeList.push_back(eComponentType::CT_PLAYER);
			Component* findComponent = map->FindComponentInRange(this, 4, findTypeList);

			// 적이 있으면 적 방향으로 이동
			eDirection currentDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
				{
					currentDirection = eDirection::UP;
				}
				else if (_tilePosition.y < targetPosition.y)
				{
					currentDirection = eDirection::DOWN;
				}
				else if (_tilePosition.x < targetPosition.x)
				{
					currentDirection = eDirection::RIGHT;
				}
				else if (targetPosition.x < _tilePosition.x)
				{
					currentDirection = eDirection::LEFT;
				}
				else
				{
					currentDirection = (eDirection)(rand() % 4);
				}
			}
			else
			{
				currentDirection = (eDirection)(rand() % 4);
			}
			if (eDirection::NONE != currentDirection)
			{
				MoveStart(currentDirection);
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

void Monster::Collision(std::vector<Component*> collisionList)
{
	for(int i = 0; i < collisionList.size(); i++)
	{
		if(eComponentType::CT_NPC == collisionList[i]->GetType() ||
			eComponentType::CT_PLAYER == collisionList[i]->GetType())
		{
			//적이면 공격
			sMessageParam param;
			param.sender = this;
			param.receiver = collisionList[i];
			param.message = L"Attack";
			param.attackPoint = _attackPoint;
			ComponentSystem::GetInstance()->SendMsg(param);
		}
	}
}