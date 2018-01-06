#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "Sprite.h"
#include "Font.h"

#include <iostream>

using namespace std;

Character::Character(std::wstring name) : Component(name)
{
	_position.x = _position.y = 0.0f;

	_moveTime = (float)(rand() % 100 + 50) / 100.0f;
	_movingDuration = 0.0f;
	_isMoving = false;

	_currentDirection = eDirection::LEFT;
	SetCanMove(false);
	// Common Info
	_hp = 50;
	_attackPoint = 10;
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
		// 이름으로 (맵인) 컴포넌트를 찾는다.
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			_tilePosition.x = rand() % map->GetWidth();
			_tilePosition.y = rand() % map->GetHeight();
			map->SetTileComponent(_tilePosition, this);
		}
	}

	// font
	{
		_font = new Font(L"Arial", 15, D3DCOLOR_ARGB(255, 0, 0, 0));
		_font->SetRect(_position.x - 100, _position.y - 30, 200, 50);

		WCHAR text[100];
		wsprintf(text, L"HP %d", _hp);
		_font->SetText(text);
	}
}

void Character::Deinit()
{

	if (NULL != _font)
	{
		delete _font;
		_font = NULL;
	}

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

	WCHAR text[100];
	wsprintf(text, L"HP %d", _hp);
	_font->SetText(text);
}

void Character::Render()
{
	_spriteList[_currentDirection]->SetPosition(_position.x, _position.y);
	_spriteList[_currentDirection]->Render();

	_font->SetPosition(_position.x-100, _position.y-30);
	_font->Render();
}

void Character::Release()
{
	_font->Release();
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Release();
	}
}

void Character::Reset()
{
	_font->Reset();
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void Character::ReceiveMsg(const sMessageParam& param)
{
	//MessageBox(0, param.message.c_str(), L"Hello", MB_OK);
	if (L"Attack" == param.message)
	{
		
		int attackPoint = param.attackPoint;
		_hp -= attackPoint;
		if (_hp < 0)
		{
			_isLive = false;
			SetCanMove(true);
		}
	}
}

void Character::UpdateAI(float deltaTime)
{
}

void Character::MoveStart(eDirection direction)
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		TilePoint newTilePosition = _tilePosition;
		switch (direction)
		{
		case eDirection::LEFT:	newTilePosition.x--; break;
		case eDirection::RIGHT:	newTilePosition.x++; break;
		case eDirection::UP:	newTilePosition.y--; break;
		case eDirection::DOWN:	newTilePosition.y++; break;
		}

		if (eDirection::NONE != direction)
			_currentDirection = direction;

		if (newTilePosition.x != _tilePosition.x ||
			newTilePosition.y != _tilePosition.y)
		{
			// Message System
			std::vector<Component*> collisionList = map->GetTileCollisionList(newTilePosition);
			if (0 < collisionList.size())
			{
				Collision(collisionList);

			}
			else
			{
				if (map->CanMoveTile(newTilePosition))
				{
					map->ResetTileComponent(_tilePosition, this);
					_tilePosition = newTilePosition;
					map->SetTileComponent(_tilePosition, this);

					// 캐릭터가 이동하면 맵도 이동시켜준다.
					_isMoving = true;
				}
			}
		}
	}
}

void Character::Collision(std::vector<Component*> collisionList)
{
	for (int i = 0; i < collisionList.size(); i++)
	{
		Component* component = collisionList[i];

		sMessageParam param;
		param.sender = this;
		param.receiver = component;
		param.message = L"Collision";
		ComponentSystem::GetInstance()->SendMsg(param);
	}
}