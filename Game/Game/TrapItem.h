
#pragma once
#include "Component.h"

class Sprite;
class TrapItem
	:public Component
{
private:
	Sprite * _sprite;
public:
	TrapItem(std::wstring name);
	~TrapItem();
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	void ReceiveMsg(const sMessageParam& param);	// 메시지 처리를 위해 재정의
};