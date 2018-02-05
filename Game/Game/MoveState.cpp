#include "ComponentSystem.h"
#include "Map.h"
#include "MoveState.h"
#include "Character.h"

MoveState::MoveState(Character* character)
	:State(character)
{
	//_character = character;
	_movingDuration = 0.0f;
	//_isMoving = false;

}

MoveState::~MoveState()
{

}
void MoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
	UpdateMove(deltaTime);
}
void MoveState::UpdateMove(float deltaTime)
{
	//_movingDuration += deltaTime;
	if (false == _character->IsLive())
	{
		return;
	}
	if (false == _character->IsMoving())
	{
		return;
	}

	if (_character->GetMoveTime() <= GetMovingDuration())
	{
		//Stop();
		//_character->ChangeState(eStateType::ST_IDLE);
		_nextState = eStateType::ST_IDLE;

	}
	else
	{
		_movingDuration += deltaTime;
	}
}


void MoveState::Start()
{
	//_nextState = eStateType::ST_NONE;
	State::Start();
	//_character->MoveStart(_character->GetNextDirection());
	Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
	if (NULL != map)
	{
		//ilePoint newTilePosition = _tilePosition;
		TilePoint newTilePosition = _character->GetTilePosition();


		switch (_character->GetNextDirection())
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
		if (eDirection::NONE != _character->GetNextDirection())
		{
			//_currentDirection = direction;
			_character->SetDirection(_character->GetNextDirection());
		}
		if (newTilePosition.x != _character->GetTilePosition().x ||
			newTilePosition.y != _character->GetTilePosition().y)
		{
			std::vector<Component*> collisionList = map->GetTileCollisionList(newTilePosition);
			if (0 < collisionList.size())
			{
				//Collision(collisionList);
				//_character->Collision(collisionList);
				// 즉시 바꾸는게 아니라 바꿀 상태를 예약 했다가 다음업데이트 때 교체
				//_character->ChangeState(eStateType::ST_IDLE);
				//_nextState = eStateType::ST_IDLE;
				std::vector<Component*> enemyList = _character->Collision(collisionList);
				if (0 < enemyList.size() && _character->IsAttackCoolTime())
				{
					_character->ResetAttackCoolTime();
					_character->SetTarget(enemyList);
					_nextState = eStateType::ST_ATTACK;
				}
				else
				{
					_nextState = eStateType::ST_IDLE;
				}
				/*
				for (int i = 0; i < collisionList.size(); i++)
				{
				Component* component = collisionList[i];

				ComponentSystem::GetInstance()->SendMsg(L"Collision", component, this);

				}
				*/
			}
			else
			{
				if (map->CanMoveTile(newTilePosition))
				{
					_character->MoveStart(newTilePosition);
					//map->ResetTileComponent(_tilePosition, this);
					//_tilePosition = newTilePosition;
					//map->setTileComponent(_tilePosition, this);
					//_isMoving = true;
					//_state->_isMoving = true;
					//_state->SetMoving(true);
					//_isMoving = true;
				}
				else
				{
					//_character->ChangeState(eStateType::ST_IDLE);
					_nextState = eStateType::ST_IDLE;

				}
				/*
				if (0 <= newTilePosition.x && newTilePosition.x < map->GetWidth() &&
				0 <= newTilePosition.y && newTilePosition.y < map->GetHeight())
				{
				//_currentDirection = currentDirection;
				map->ResetTileComponent(_tilePosition, this);
				_tilePosition = newTilePosition;
				map->setTileComponent(_tilePosition, this);


				_isMoving = true;
				}
				*/
			}
		}
		else
		{
			//_character->ChangeState(eStateType::ST_IDLE);
			_nextState = eStateType::ST_IDLE;

		}
	}
}
void MoveState::Stop()
{
	_movingDuration = 0.0f;
	//isMoving = false;
	_character->MoveStop();
}

