#include "Sprite.h"
#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "Font.h"
#include "MoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"
#include "TileCell.h"
Character::Character(std::wstring name) : Component(name)
{
	_position.x = _position.y = 0.0f;

	_state = NULL;
	_isMoving = false;
	_moveTime = (float)(rand() % 100 + 50) / 100.0f;

	_currentDirection = eDirection::LEFT;
	SetCanMove(false);

	_hp = 100;
	_attackPoint = 10;
	_attackCoolTimeDuration = 0.0f;
	_attackCoolTime = 1.0f;
}

Character::~Character()
{
}

void Character::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// 화면에 그려질 스프라이트 생성
	/*
	_sprite = new Sprite();
	_sprite->Init(textureFilename,scriptFilename);
	*/
	/*
	_spriteList.clear();
	{
	// left
	std::wstring script = scriptFilename;
	script = script.append(L"_left.json");
	Sprite *sprite = new Sprite();
	sprite->Init(textureFilename, script);
	_spriteList.push_back(sprite);
	}
	{
	// right
	std::wstring script = scriptFilename;
	script = script.append(L"_right.json");
	Sprite *sprite = new Sprite();
	sprite->Init(textureFilename, script);
	_spriteList.push_back(sprite);
	}
	{
	// up
	std::wstring script = scriptFilename;
	script = script.append(L"_up.json");
	Sprite *sprite = new Sprite();
	sprite->Init(textureFilename, script);
	_spriteList.push_back(sprite);
	}
	{
	// down
	std::wstring script = scriptFilename;
	script = script.append(L"_down.json");
	Sprite *sprite = new Sprite();
	sprite->Init(textureFilename, script);
	_spriteList.push_back(sprite);
	}
	*/
	// 타일인덱스를 통한 위치 세팅 테스트
	{
		// 픽셀 좌표
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			TilePoint tilePos;
			tilePos.x = rand() % map->GetWidth();
			tilePos.y = rand() % map->GetHeight();
			while (false == map->GetTileCell(tilePos)->CanMove())
			{
				tilePos.x = rand() % map->GetWidth();
				tilePos.y = rand() % map->GetHeight();
			}
			_tilePosition = tilePos;

			map->setTileComponent(_tilePosition, this);
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

	InitState(textureFilename, scriptFilename);
	ChangeState(eStateType::ST_IDLE);
}

void Character::Deinit()
{
	for (std::map<eStateType, State*>::iterator it = _stateMap.begin();
		it != _stateMap.end(); it++)
	{
		State* state = it->second;
		delete state;
	}
	_stateMap.clear();

	if (NULL != _font)
	{
		delete _font;
		_font = NULL;
	}
}

void  Character::Update(float deltaTime)
{
	_state->Update(deltaTime);
	UpdateAttackCoolTime(deltaTime);

	int coolTime = (int)(_attackCoolTimeDuration * 1000.0f);
	WCHAR text[100];
	wsprintf(text, L"HP %d\n%d", _hp, coolTime);
	_font->SetText(text);

}
void Character::Render()
{
	//_sprite->SetPosition(_position.x, _position.y);
	//_sprite->Render();

	//_spriteList[_currentDirection]->SetPosition(_position.x, _position.y);
	//_spriteList[_currentDirection]->Render();
	_state->Render();
	_font->SetPosition(_position.x - 100, _position.y - 50);
	_font->Render();
}
void Character::Release()
{
	_font->Release();
	_state->Release();
	//_sprite->Release();
	/*
	for (int i = 0; i < _spriteList.size(); i++)
	{
	_spriteList[i]->Release();
	}
	*/
}
void Character::Reset()
{
	_font->Reset();
	_state->Reset();
	//_sprite->Reset();
	/*
	for (int i = 0; i < _spriteList.size(); i++)
	{
	_spriteList[i]->Reset();
	}
	*/
}

void Character::ReceiveMsg(const sMessageParam& param)
{
	if (L"Attack" == param.message)
	{
		_damagePoint = param.attackPoint;
		_state->ChangeState(eStateType::ST_DEFFENSE);
	}
	if (L"RecoveryHP" == param.message)
	{
		//RecoverHp(param.recoveryHP);
		_hp += param.recoveryHP;
		if (100 < _hp)
			_hp = 100;
	}

	if (L"AttakTrap" == param.message)
	{
		_damagePoint = param.attackPoint;
		_state->ChangeState(eStateType::ST_DEFFENSE);
	}
}
void Character::InitState(std::wstring textureFilename, std::wstring scriptFilename)
{
	//StateMap 구성
	// 크기를 몇인지 모르고 임의 접근을 할 수 없기 때문에 링크드,큐,스택 사용 불가
	// 배열은 크기를 모르고 , 순서가 바뀌였을 때는 문제가 되기 때문에 사용 할 수 없다.
	// Map 사용
	{
		State* state = new IdleState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_IDLE] = state;
	}
	{
		State* state = new MoveState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_MOVE] = state;
	}
	{
		State* state = new AttackState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_ATTACK] = state;
	}
	{
		State* state = new DefenseState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_DEFFENSE] = state;
	}
	{
		State* state = new DeadState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_DIE] = state;
	}
}

void Character::ChangeState(eStateType stateType)
{
	// 기존 상태는 스톱
	if (NULL != _state)
	{
		_state->Stop();
	}

	// 바뀐 상태를 스타트
	_state = _stateMap[stateType];
	_state->Start();
}

void Character::UpdateAI(float deltaTime)
{

}

void Character::MoveStart(TilePoint newTilePosition)
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		map->ResetTileComponent(_tilePosition, this);
		_tilePosition = newTilePosition;
		map->setTileComponent(_tilePosition, this);

		_isMoving = true;
		/*
		TilePoint newTilePosition = _tilePosition;

		switch (direction)
		{
		case eDirection::LEFT:
		newTilePosition.x--;
		break;
		case eDirection::RIGHT:
		newTilePosition.x++;
		break;
		case eDirection::UP:
		newTilePosition.y--;
		break;
		case eDirection::DOWN:
		newTilePosition.y++;
		break;
		}
		if (eDirection::NONE != direction)
		{
		_currentDirection = direction;
		}
		if (newTilePosition.x != _tilePosition.x ||
		newTilePosition.y != _tilePosition.y)
		{
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
		map->setTileComponent(_tilePosition, this);
		//_isMoving = true;
		//_state->_isMoving = true;
		_state->SetMoving(true);
		}

		}
		}
		*/
	}
}

void Character::MoveStop()
{
	_isMoving = false;
}

std::vector<Component*> Character::Collision(std::vector<Component*> collisionList)
{
	/*
	for (int i = 0; i < collisionList.size(); i++)
	{
	Component* component = collisionList[i];

	sMessageParam param;
	param.sender = this;
	param.receiver = component;
	param.message = L"Collision";
	ComponentSystem::GetInstance()->SendMsg(param);
	//ComponentSystem::GetInstance()->SendMsg(L"Collision", component, this);

	}
	*/

	return collisionList;
}

TileCell* Character::PopPathfindingCell()
{
	TileCell* tileCell = _pathfindingCellStack.top();
	_pathfindingCellStack.pop();
	return tileCell;
}

bool Character::IsEmptyPathfindingStack()
{
	if (0 != _pathfindingCellStack.size())
		return false;
	return true;
}

void Character::UpdateAttackCoolTime(float deltaTime)
{
	if (_attackCoolTimeDuration < _attackCoolTime)
		_attackCoolTimeDuration += deltaTime;
}

bool Character::IsAttackCoolTime()
{
	if (_attackCoolTime <= _attackCoolTimeDuration)
		return true;
	return false;
}

void Character::ResetAttackCoolTime()
{
	_attackCoolTimeDuration = 0.0f;
}

void Character::DecreaseHP(int damagePoint)
{
	_hp -= damagePoint;

	if (_hp < 0)
	{
		_isLive = false;
		_hp = 0;
	}
}

void Character::EatItem()
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		// 발 밑에 아이템이 있으면
		Component* item = map->FindItemInTile(_tilePosition);
		if (NULL != item)
		{
			sMessageParam msg;
			msg.sender = this;
			msg.receiver = item;
			msg.message = L"USE";
			ComponentSystem::GetInstance()->SendMsg(msg);
		}

	}
}