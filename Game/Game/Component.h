#pragma once

#include <string>
#include "GlobalTypes.h"

class ComponentSystem;

class Component
{
protected:
	std::wstring _name;
	Point _position;

public:
	Component(std::wstring name);
	virtual ~Component();

public:
	// =0 을 붙이면, 추상 가상함수가 되기 때문에, Component를 상속받는
	// 모든 객체는 무조건적으로 추상 가상함수를 선언과 정의를 해줘야한다.
	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
	virtual void Reset() = 0;

	void SetPosition(Point position) { _position = position; }
};