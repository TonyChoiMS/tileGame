#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"

#include "Sprite.h"

Character::Character(std::wstring name) : Component(name)
{
	_position.x = _position.y = 0.0f;

	_moveTime = (float)(rand() % 100 + 50) / 100.0f;
	_movingDuration = 0.0f;
	_isMoving = false;

	_currentDirection = eDirection::LEFT;
	SetCanMove(false);
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