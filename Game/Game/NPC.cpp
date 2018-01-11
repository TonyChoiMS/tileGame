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
			//NPC�� Player�� ���󰡴� ������ AI (��ã�� ���ʹ���)

			// Ž�� - ���� ���� ���� ���� �ִ°�?
			std::vector<eComponentType>findTypeList;
			findTypeList.push_back(eComponentType::CT_MONSTER);
			Component* findComponent = map->FindComponentInRange(this, 4, findTypeList);

			// ���� ������ ����
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

				// �� �� ���� �����̸� ������(���� ������ ���� �ϰ� ������ ������ �˻��Ѵ�.)
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