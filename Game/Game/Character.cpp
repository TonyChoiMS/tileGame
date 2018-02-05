#include "Sprite.h"
#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "Font.h"
#include "MoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DieState.h"
#include "TileCell.h"
Character::Character(std::wstring name)
	:Component(name)
{

	//_sprite = NULL;
	//_x = _y = 0.0f;
	//_isMoving = false;

	//moveTime = 0.3f;
	//_moveTime = (float)(rand() % 100 + 50) / 100.0f;
	//_state->_moveTime = 0.3f;
	//_state = new IdleState(this);

	//_state = new IdleState(this);
	_state = NULL;
	_isMoving = false;
	_moveTime = (float)(rand() % 100 + 50) / 100.0f;

	_position.x = _position.y = 0.0f;
	_currentDirection = eDirection::LEFT;
	SetCanMove(false);

	_hp = 30;
	_attackPoint = 10;
	_attackCoolTimeDuration = 0.0f;
	_attackCoolTime = 3.0f;
}
Character::~Character()
{

}
void Character::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// ȭ�鿡 �׷��� ��������Ʈ ����
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
	// Ÿ���ε����� ���� ��ġ ���� �׽�Ʈ
	{
		//_tilePosition.x = 1;
		//_tilePosition.y = 1;

		// �ȼ� ��ǥ
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
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
			/*_tilePosition.x = rand() % map->GetWidth();
			_tilePosition.y = rand() % map->GetHeight();*/
			map->setTileComponent(_tilePosition, this);
			//_position = map->GetPosition(tileX, tileY);
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


	//StateMap ����
	// ũ�⸦ ������ �𸣰� ���� ������ �� �� ���� ������ ��ũ��,ť,���� ��� �Ұ�
	// �迭�� ũ�⸦ �𸣰� , ������ �ٲ�� ���� ������ �Ǳ� ������ ��� �� �� ����.
	// Map ���
	/*
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
	State* state = new DieState(this);
	state->CreateSprite(textureFilename, scriptFilename);
	_stateMap[eStateType::ST_DIE] = state;
	}
	*/
	InitState(textureFilename, scriptFilename);

	ChangeState(eStateType::ST_IDLE);
}
void Character::Deinit()
{
	/*
	if (NULL != _sprite)
	{
	delete _sprite;
	_sprite = NULL;
	}
	*/
	/*
	if (NULL != _state)
	{
	delete _state;
	_state = NULL;
	}
	*/
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
	/*
	for(int i = 0; i < _spriteList.size(); i++)
	{
	delete _spriteList[i];
	}
	_spriteList.clear();
	*/
}
void  Character::Update(float deltaTime)
{
	//_sprite->Update(deltaTime);

	//_spriteList[_currentDirection]->Update(deltaTime);
	//UpdateAI(deltaTime);
	//UpdateMove(deltaTime);
	_state->Update(deltaTime);
	UpdateAttackCoolTime(deltaTime);
	int coolTime = (int)(_attackCoolTimeDuration * 10000.0f);
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
		/*
		int attackPoint = param.attackPoint;
		_hp -= attackPoint;
		if (0 >_hp)
		{
		_isLive = false;
		SetCanMove(true);
		}
		*/
	}
	if (L"RecoveryHP" == param.message)
	{
		//RecoverHp(param.recoveryHP);
		_hp += param.recoveryHP;
		if (100 < _hp)
		{
			_hp = 100;
		}
	}

	if (L"AttakTrap" == param.message)
	{
		_damagePoint = param.attackPoint;
		_state->ChangeState(eStateType::ST_DEFFENSE);
	}
}
void Character::InitState(std::wstring textureFilename, std::wstring scriptFilename)
{
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
		State* state = new DieState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_DIE] = state;
	}
}
void Character::ChangeState(eStateType stateType)
{
	// ���� ���´� ����
	if (NULL != _state)
	{
		_state->Stop();
	}
	// Ÿ�Կ� ���缭 ���ο� ���·� ��ü
	/*
	switch (stateType)
	{
	case eStateType::ST_IDLE:
	_state = new IdleState(this);
	break;
	case eStateType::ST_MOVE:
	_state = new MoveState(this);
	break;
	default:
	break;
	}
	*/
	_state = _stateMap[stateType];


	// �ٲ� ���¸� ��ŸƮ
	_state->Start();
}
void Character::DecreaseHP(int damagePoint)
{
	_hp -= damagePoint;

	if (_hp < 0)
	{
		_isLive = false;
		_hp = 0;
	}

	/*
	if (_hp < 0)
	{
	_state->ChangeState(eStateType::ST_DIE);
	}
	*/


}
/*
void Character::IsDie()
{
_isLive = false;
_hp = 0;
}
*/
void Character::UpdateAI(float deltaTime)
{

}

void Character::MoveStart(TilePoint newTilePosition)
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
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

void Character::UpdateMove(float deltaTime)
{
	/*
	if (false == _isLive)
	{
	return;
	}
	if (false == _isMoving)
	{
	return;
	}

	if (_moveTime <= _state->GetMovingDuration())
	{
	_state->Stop();
	}
	else
	{
	_state->UpdateMove(deltaTime);
	}
	*/
}

bool Character::IsAttackCoolTime()
{
	if (_attackCoolTime <= _attackCoolTimeDuration)
	{
		return true;
	}
	return false;
}


void Character::ResetAttackCoolTime()
{
	_attackCoolTimeDuration = 0.0f;
}

void Character::UpdateAttackCoolTime(float deltaTime)
{
	if (_attackCoolTimeDuration < _attackCoolTime)
	{
		_attackCoolTimeDuration += deltaTime;
	}
}

void Character::EatItem()
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
	if (NULL != map)
	{
		// �� �ؿ� �������� ������
		Component* item = map->FindItemInTile(_tilePosition);
		if (NULL != item)
		{
			/*
			// �� �������� ���
			_hp = 100;
			// ������ ����
			map->ResetTileComponent(_tilePosition, item);
			item->SetLive(false);
			*/
			sMessageParam msg;
			msg.sender = this;
			msg.receiver = item;
			msg.message = L"USE";
			ComponentSystem::GetInstance()->SendMsg(msg);
		}

	}
}

void Character::PushPathfindingCell(TileCell* tileCell)
{
	_pathfindingCellStack.push(tileCell);
}

bool Character::IsEmptyPathfindingStack()
{
	if (0 != _pathfindingCellStack.size())
	{
		return false;
	}
	return true;
}
TileCell* Character::PopPathfindingCell()
{
	TileCell* tileCell = _pathfindingCellStack.top();
	_pathfindingCellStack.pop();
	return tileCell;
}

