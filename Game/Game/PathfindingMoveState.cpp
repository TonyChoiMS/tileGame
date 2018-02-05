#include "TileCell.h"
#include "Character.h"
#include "PathfindingMoveState.h"

PathfindingMoveState::PathfindingMoveState(Character* character)
	:State(character)
{

}
PathfindingMoveState::~PathfindingMoveState()
{

}
void PathfindingMoveState::Start()
{
	State::Start();

	_movingDuration = _character->GetMoveTime();
	// 맨 마지막에 들어간 타일셀은 버린다.
	TileCell* tileCell = _character->PopPathfindingCell();
}
void PathfindingMoveState::Stop()
{
	State::Stop();
	_character->MoveStop();
}
void PathfindingMoveState::Update(float deltaTime)
{
	// 스택을 사용
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsEmptyPathfindingStack())
	{
		if (_character->GetMoveTime() <= _movingDuration)
		{
			_movingDuration = 0.0f;
			// Character move
			TileCell* tileCell = _character->PopPathfindingCell();

			TilePoint toPosition = tileCell->GetTilePosition();
			TilePoint fromPosition = _character->GetTilePosition();
			eDirection direction = GetDirection(toPosition, fromPosition);
			if (eDirection::NONE != direction)
			{
				_character->SetDirection(direction);
			}

			_character->MoveStart(toPosition);
		}
	}
	else
	{
		_nextState = eStateType::ST_IDLE;
	}
	_movingDuration += 0.0003f;
}

eDirection PathfindingMoveState::GetDirection(TilePoint to, TilePoint from)
{
	if (from.x < to.x)
	{
		return eDirection::RIGHT;
	}
	if (from.x > to.x)
	{
		return eDirection::LEFT;
	}

	if (from.y < to.y)
	{
		return eDirection::DOWN;
	}
	if (from.y > to.y)
	{
		return eDirection::UP;
	}
	return eDirection::NONE;

}