#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"

#include "Sprite.h"

Character::Character(std::wstring name) : Component(name)
{
	_position.x = _position.y = 0.0f;

	_moveTime = 0.5f;
	_movingDuration = 0.0f;
	_isMoving = false;

	_currentDirection = eDirection::LEFT;
}

Character::~Character()
{

}

void Character::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// ȭ�鿡 �׷��� ��������Ʈ ����
	_spriteList.clear();
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_left.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_right.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_up.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_down.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	// Ÿ�� �ε����� ���� ��ġ ���� �׽�Ʈ
	{
		_tilePosition.x = 1;
		_tilePosition.y = 1;

		// �̸����� (����) ������Ʈ�� ã�´�.
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			map->SetTileComponent(_tilePosition, this);
		}
	}
}

void Character::Deinit()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Character::Update(float deltaTime)
{
	_spriteList[_currentDirection]->Update(deltaTime);
	UpdateAI(deltaTime);
}

void Character::Render()
{
	_spriteList[_currentDirection]->SetPosition(_position.x, _position.y);
	_spriteList[_currentDirection]->Render();
}

void Character::Release()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Release();
	}
}

void Character::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void Character::UpdateAI(float deltaTime)
{
}
