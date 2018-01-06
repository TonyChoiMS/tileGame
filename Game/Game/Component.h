#pragma once

#include <string>
#include "GlobalTypes.h"
#include "ComponentMessage.h"

enum eComponentType
{
	CT_DEFAULT,
	CT_PLAYER,
	CT_NPC,
	CT_MONSTER
};
class Component
{
protected:
	std::wstring _name;
	Point _position;
	eComponentType _type;
	bool _isLive;

public:
	Component(std::wstring name);
	virtual ~Component();
	eComponentType GetType() { return _type; }
	bool IsLive() { return _isLive; }
	void SetIsLive(bool isLive) { _isLive = isLive; }

public:
	// =0 을 붙이면, 추상 가상함수가 되기 때문에, Component를 상속받는
	// 모든 객체는 무조건적으로 추상 가상함수를 선언과 정의를 해줘야한다.
	virtual void Init(std::wstring textureFilename, std::wstring scriptFilename) = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
	virtual void Reset() = 0;

	void SetPosition(Point position) { _position = position; }

protected:
	TilePoint _tilePosition;

public:
	TilePoint GetTilePosition() { return _tilePosition; }

	// Message
public:
	virtual void ReceiveMsg(const sMessageParam& param);

	//Move
private:
	bool _canMove;

public:
	bool CanMove() { return _canMove; }
	void SetCanMove(bool canMove) { _canMove = canMove; }
};