#pragma once
#include <string>
#include <d3dx9.h>
#include <vector>
#include <map>
#include <stack>
#include "GlobalTypes.h"
#include "Component.h"


class Sprite;
class Font;
class TileCell;
//class MoveState;
//class IdleState;
class State;

enum eStateType
{
	ST_NONE,
	ST_IDLE,
	ST_MOVE,
	ST_ATTACK,
	ST_DEFFENSE,
	ST_PATHFINDING,
	ST_DIE
};

class Character
	:public Component
{
protected:
	//
	//std::vector<Sprite*> _spriteList;
	//AI
	/*
	bool _isMoving;
	float _moveTime;
	float _movingDuration;
	*/
	//
	eDirection _currentDirection;

public:
	Character(std::wstring name);
	~Character();
public:
	//void Init(IDirect3DDevice9* device3d, ID3DXSprite* spriteDX);

	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	//void Reset(IDirect3DDevice9* device3d, ID3DXSprite* spriteDX);

	void Reset();
	//Message
protected:
	int _hp;
	int _attackPoint;
public:
	void ReceiveMsg(const sMessageParam& param);

	// AI
protected:
	float _moveTime;
	bool _isMoving;
	std::vector<Component*> _targetList;
	//MoveState * _state;
	//IdleState* _state;
	eDirection _nextDirection;

	// State
protected:
	std::map<eStateType, State*> _stateMap;
	State* _state;
public:
	virtual void InitState(std::wstring textureFilename, std::wstring scriptFilename);
	void ChangeState(eStateType stateType);
	int GetAttackPoint() { return _attackPoint; }
	// Damage
private:
	int _damagePoint;
public:
	int GetDamagePoint() { return _damagePoint; }
	void DecreaseHP(int damagePoint);
	// DIe
public:
	//void IsDie();
public:
	virtual void UpdateAI(float deltaTime);
	//void MoveStart(eDirection currentDirection);
	void MoveStart(TilePoint newTilePosition);
	void MoveStop();
	void UpdateMove(float deltaTime);

	//void SetMoving(bool isMoving) { _isMoving = isMoving; }
	bool IsMoving() { return _isMoving; }
	float GetMoveTime() { return _moveTime; }

	virtual std::vector<Component*> Collision(std::vector<Component*> collisionList);

	std::vector<Component*> GetTargetList() { return _targetList; }
	void ResetTarget() { _targetList.clear(); }
	void SetTarget(std::vector<Component*> targetList) { _targetList = targetList; }
	void SetDirection(eDirection direction) { _currentDirection = direction; }
	eDirection GetDirection() { return _currentDirection; }
	eDirection GetNextDirection() { return _nextDirection; }

	// CoolTime
protected:
	float _attackCoolTimeDuration;
	float _attackCoolTime;

public:
	bool IsAttackCoolTime();
	void ResetAttackCoolTime();
	void UpdateAttackCoolTime(float deltaTime);

	// item
public:
	void EatItem();

	// UI

private:
	Font* _font;
	// Pathfinding 
protected:
	TileCell * _targetTileCell;
public:
	void SetTargetTileCell(TileCell* targetTileCell) { _targetTileCell = targetTileCell; }
	TileCell* GetTargetTileCell() { return _targetTileCell; }

protected:
	std::stack<TileCell*> _pathfindingCellStack;
public:
	void PushPathfindingCell(TileCell* tileCell) { _pathfindingCellStack.push(tileCell); }
	bool IsEmptyPathfindingStack();
	TileCell* PopPathfindingCell();

};