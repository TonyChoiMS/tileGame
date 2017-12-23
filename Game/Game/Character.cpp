#include "Character.h"

Character::Character(std::wstring name) 
{
	_sprite = NULL;
	_x = _y = 0.0f;
}

Character::~Character()
{

}

void Character::Init(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	// ȭ�鿡 �׷��� ��������Ʈ ����
	_sprite = new Sprite(device3d, spriteDX);
	_sprite->Init(L"character_sprite.png", L"player_down.json");
}

void Character::Deinit()
{
	if (_sprite != NULL) {
		delete _sprite;
		_sprite = NULL;
	}
}

void Character::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void Character::Render()
{
	_sprite->SetPosition(_x, _y);
	_sprite->Render();
}

void Character::Release()
{
	_sprite->Release();
}

void Character::Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_sprite->Reset(device3d, spriteDX);
}
