#pragma once
#include  <string>
#include "GlobalTypes.h"
#include "ComponentMessage.h"


enum eComponentType
{
	CT_DEFAULT,
	CT_PLAYER,
	CT_NPC,
	CT_MONSTER,
	CT_ITEM

};
class Component
{
private:
	bool _canMove;
protected:
	eComponentType _type;
	std::wstring _name;
	Point _position;
	TilePoint _tilePosition;
	bool _isLive;

public:
	Component(std::wstring name);
	virtual ~Component();
public:

	// �߻� ���� �Լ��� 0 �� �������ָ� ������Ʈ�� 
	//��ӹ޴� ��� Ŭ������ ������ ���� �� ���� ����� �Ѵ�.
	virtual void Init(std::wstring textureFilename, std::wstring scriptFilename) = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
	virtual void Reset() = 0;

public:
	void SetPosition(Point postion) { _position = postion; }
	Point GetPosition() { return _position; }
	TilePoint GetTilePosition() { return _tilePosition; }
	//Message
public:
	virtual void ReceiveMsg(const sMessageParam& param);

public:
	// move
	bool CanMove() { return _canMove; }
	void SetCanMove(bool canMove) { _canMove = canMove; } // ��������.
	eComponentType GetType() { return _type; }

	//
	bool IsLive() { return _isLive; }
	void SetLive(bool isLive) { _isLive = isLive; }
};