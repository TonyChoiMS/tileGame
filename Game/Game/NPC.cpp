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
	{
		return;
	}

	//if (false == _state->_isMoving)
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		if (NULL != map)
		{
			//NPC와 Player를 따라가는 간단한 AI (길찾기 기초버전)
			// 탐색 - 일정 범위 내에 적이 있는가?
			std::vector<eComponentType>findTypeList;
			findTypeList.push_back(eComponentType::CT_MONSTER);
			Component* findComponent = map->FindComponentInRange(this, 2, findTypeList);
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
			// 적이 있으면 도망
			_nextDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
				{
					_nextDirection = eDirection::DOWN;
				}
				else if (_tilePosition.y < targetPosition.y)
				{
					_nextDirection = eDirection::UP;
				}
				else if (_tilePosition.x < targetPosition.x)
				{
					_nextDirection = eDirection::LEFT;
				}
				else if (targetPosition.x < _tilePosition.x)
				{
					_nextDirection = eDirection::RIGHT;
				}

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
					{
						break;
					}

					_nextDirection = (eDirection)(((int)_nextDirection + 1) % ((int)eDirection::NONE));
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
			/*
			TilePoint newTilePosition = _tilePosition;
			//int direction = rand() % 4;
			_currentDirection = (eDirection)(rand() % 4);
			switch (_currentDirection)
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

			if (0 < newTilePosition.x && newTilePosition.x < map->GetWidth() &&
			0 <= newTilePosition.y && newTilePosition.y < map->GetHeight())
			{
			map->ResetTileComponent(_tilePosition, this);
			_tilePosition = newTilePosition;
			map->setTileComponent(_tilePosition, this);

			_isMoving = true;
			}
			*/
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
