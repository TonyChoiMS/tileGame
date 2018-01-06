#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include "GlobalTypes.h"
#include "Component.h"

class Sprite;
class Font;

class Character : public Component
{
protected:
	std::vector<Sprite*> _spriteList;
	eDirection _currentDirection;

	//TilePoint _tilePosition;		// Component Level·Î »ó½Â

public:
	Character(std::wstring name);
	~Character();

	void Init(std::wstring textureFilename, std::wstring scriptFilename) override;
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	// Common Info
protected:
	int _hp;
	int _attackPoint;

	// Message
public:
	void ReceiveMsg(const sMessageParam& param);

	// AI
protected:
	float _moveTime;
	float _movingDuration;
	bool _isMoving;

public:
	virtual void UpdateAI(float deltaTime);
	void MoveStart(eDirection direction);

	virtual void Collision(std::vector<Component*> collisionList);

	// UI
private:
	Font* _font;
};
