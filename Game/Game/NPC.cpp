#include "NPC.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "MoveState.h"
#include "IdleState.h"
NPC::NPC(std::wstring name)
	: Character(name)
{
	_type = eComponentType::CT_NPC;
	//SetCanMove(true);
}

NPC::~NPC()
{
}

void NPC::UpdateAI(float deltaTime)
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
			std::vector<eComponentType> findTypeList;
			findTypeList.push_back(eComponentType::CT_MONSTER);
			Component* findComponent = map->FindComponentInRange(this, 2, findTypeList);
		
			// 적이 있으면 도망
			_nextDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
					_nextDirection = eDirection::DOWN;
				else if (_tilePosition.y < targetPosition.y)
					_nextDirection = eDirection::UP;
				else if (_tilePosition.x < targetPosition.x)
					_nextDirection = eDirection::LEFT;
				else if (targetPosition.x < _tilePosition.x)
					_nextDirection = eDirection::RIGHT;

				// 갈 수 없는 방향이면 재조정(현재 방향을 제외 하고 나머지 방향을 검사한다.)
				TilePoint nextPosition = _tilePosition;
				for (int i = 0; i < 4; i++)
				{
					switch (_nextDirection)
					{
					case eDirection::LEFT:
						nextPosition.x = _tilePosition.x - 1;
						break;
					case eDirection::RIGHT:
						nextPosition.x = _tilePosition.x + 1;
						break;
					case eDirection::UP:
						nextPosition.y = _tilePosition.y - 1;
						break;
					case eDirection::DOWN:
						nextPosition.y = _tilePosition.y + 1;
						break;
					}

					if (map->CanMoveTile(nextPosition))
						break;

					_nextDirection = (eDirection)(((int)_nextDirection + 1) % ((int)eDirection::NONE));
				}

			}
			else
			{
				_nextDirection = (eDirection)(rand() % 4);
			}
			if (eDirection::NONE != _nextDirection)
			{
				_state->ChangeState(eStateType::ST_MOVE);
			}
		}
	}
}

std::vector<Component*> NPC::Collision(std::vector<Component*> collisionList)
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
