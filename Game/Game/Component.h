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
	// =0 �� ���̸�, �߻� �����Լ��� �Ǳ� ������, Component�� ��ӹ޴�
	// ��� ��ü�� ������������ �߻� �����Լ��� ����� ���Ǹ� ������Ѵ�.
	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
	virtual void Reset() = 0;

	void SetPosition(Point position) { _position = position; }
};