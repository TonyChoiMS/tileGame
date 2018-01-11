#include "ComponentSystem.h"
#include "Map.h"

#include "NPC.h"

NPC::NPC(std::wstring name) : Character(name)
{
	_type = eComponentType::CT_NPC;
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
			std::vector<eComponentType>findTypeList;
			findTypeList.push_back(eComponentType::CT_MONSTER);
			Component* findComponent = map->FindComponentInRange(this, 4, findTypeList);

			// 적이 있으면 도망
			eDirection currentDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
				{
					currentDirection = eDirection::DOWN;
				}
				else if (_tilePosition.y < targetPosition.y)
				{
					currentDirection = eDirection::UP;
				}
				else if (_tilePosition.x < targetPosition.x)
				{
					currentDirection = eDirection::LEFT;
				}
				else if (targetPosition.x < _tilePosition.x)
				{
					currentDirection = eDirection::RIGHT;
				}

				// 갈 수 없는 방향이면 재조정(현재 방향을 제외 하고 나머지 방향을 검사한다.)
				TilePoint nextPosition = _tilePosition;
				for (int i = 0; i < 3; i++)
				{
					currentDirection = (eDirection)(((int)currentDirection + 1) % (int)eDirection::NONE);
					switch (currentDirection)
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
					{
						break;
					}
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