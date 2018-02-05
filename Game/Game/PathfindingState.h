#pragma once
#include <queue>

#include "State.h"
#include "GlobalTypes.h"

class Character;
class TileCell;
class Map;

class PathfindingState
	:public State
{
protected:
	struct sPathCommand
	{
		TileCell* tileCell;
		float heuristic;	// �� ���� �������� �� ���� �˻��ؾ� �� Ÿ���̴�.
	};

	enum eUpdateState
	{
		PATHFINDING,
		BUILD_PATH
	};
	eUpdateState _updateState;

	struct compare
	{
		bool operator()(sPathCommand& a, sPathCommand& b)
		{
			return a.heuristic > b.heuristic;
		}
	};

	// FIFO 
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingQueue;
	TileCell* _targetTileCell;

	TileCell* _reverseTileCell;

	void UpdatePathfinding();
	void UpdateBuildPath();

	Map* _map;
public:
	PathfindingState(Character* character);
	~PathfindingState();
public:
	void Start();
	void Stop();
	void Update(float deltaTime);

	// pathfinding
	TilePoint GetSearchTilePositionByDirection(TilePoint tilePosition, eDirection direction);

	float CalcSimpleHeuristic(TileCell* current, TileCell* search, TileCell* goal);
	float CalcComplexHeuristic(TileCell* search, TileCell* goal);
	float CalcAStarHeuristic(float distanceFromStart, TileCell* search, TileCell* goal);

	void CheckTestMark(TileCell* tileCell);
	void CheckBuildTestMark(TileCell* tileCell);

};