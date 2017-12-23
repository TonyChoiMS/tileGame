#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"

Character::Character(std::wstring name) : Component(name)
{
	_sprite = NULL;
	_position.x = _position.y = 0.0f;

	_movingDuration = 0.0f;
	_moveTime = 0.5f;
	_isMoving = false;
}

Character::~Character()
{

}

void Character::Init()
{
	// 화면에 그려질 스프라이트 생성
	_sprite = new Sprite();
	_sprite->Init(L"character_sprite.png", L"player_down.json");

	// 타일 인덱스를 통한 위치 세팅 테스트
	{
		_tilePosition.x = 1;
		_tilePosition.y = 1;

		// 이름으로 (맵인) 컴포넌트를 찾는다.
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			map->SetTileComponent(_tilePosition, this);
			//_position = map->GetPosition(tileX, tileY);
		}
	}
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
	UpdateAI(deltaTime);
}

void Character::Render()
{
	_sprite->SetPosition(_position.x, _position.y);
	_sprite->Render();
}

void Character::Release()
{
	_sprite->Release();
}

void Character::Reset()
{
	_sprite->Reset();
}

void Character::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			TilePoint newTilePosition = _tilePosition;
			int direction = rand() % 4;
			switch (direction)
			{
			case 0: // left
				newTilePosition.x--;
				break;
			case 1: // right
				newTilePosition.x++;
				break;
			case 2: // up
				newTilePosition.y--;
				break;
			case 3: // down
				newTilePosition.y++;
				break;
			}

			if (0 <= newTilePosition.x && newTilePosition.x < map->GetWidth() &&
				0 <= newTilePosition.y && newTilePosition.y < map->GetHeight())
			{
				map->ResetTileComponent(_tilePosition, this);
				_tilePosition = newTilePosition;
				map->SetTileComponent(_tilePosition, this);

				_isMoving = true;
			}
		}
	}
	else
	{
		if (_moveTime <= _movingDuration)
		{
			_movingDuration = 0.0f;
			_isMoving = false;
		}
		else 
		{
			_movingDuration += deltaTime;
		}
	}
}
