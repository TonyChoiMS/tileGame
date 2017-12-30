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

	_currentDirection = 0;
}

Character::~Character()
{

}

void Character::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// 화면에 그려질 스프라이트 생성
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
	// 타일 인덱스를 통한 위치 세팅 테스트
	{
		_tilePosition.x = 1;
		_tilePosition.y = 1;

		// 이름으로 (맵인) 컴포넌트를 찾는다.
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
