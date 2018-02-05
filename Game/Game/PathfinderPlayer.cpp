#include "Map.h"
#include "State.h"
#include "PathfindingIdeState.h"
#include "PathfinderPlayer.h"
#include "ComponentSystem.h"
#include "PathfindingState.h"
#include "PathfindingMoveState.h"
#include "PathfindingImmedateState.h"
PathfinderPlayer::PathfinderPlayer(std::wstring name)
	:Player(name)
{

}
PathfinderPlayer::~PathfinderPlayer()
{

}

void PathfinderPlayer::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	Player::Init(textureFilename, scriptFilename);
	/*
	Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
	if (NULL != map)
	{
		map->ResetTileComponent(_tilePosition, this);

		_tilePosition.x = 7;
		_tilePosition.y = map->GetHeight() / 3;
		map->setTileComponent(_tilePosition, this);
	}
	*/
}
void PathfinderPlayer::UpdateAI(float deltaTime)
{

}

void PathfinderPlayer::InitState(std::wstring textureFilename, std::wstring scriptFilename)
{
	Player::InitState(textureFilename, scriptFilename);

	// Idle 교체 , 새로운 상태(Pathfinding  추가)
	{
		State* state = new PathfindingIdeState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_IDLE] = state;
	}
	{
		// 탐색과 경로도출
		//State* state = new PathfindingState(this);
		State* state = new PathfindingImmedateState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_PATHFINDING] = state;
	}

	{
		State* state = new PathfindingMoveState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_MOVE] = state;
	}
}