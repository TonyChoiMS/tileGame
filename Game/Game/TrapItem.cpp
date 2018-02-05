#include "ComponentSystem.h"
#include "Map.h"
#include "Sprite.h"
#include "TrapItem.h"

TrapItem::TrapItem(std::wstring name)
	:Component(name)
{
	_type = eComponentType::CT_ITEM;
}
TrapItem::~TrapItem()
{

}
void TrapItem::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// 맵상에 랜덤한 위치에 세팅
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		if (NULL != map)
		{
			_tilePosition.x = rand() % map->GetWidth();
			_tilePosition.y = rand() % map->GetHeight();


			map->setTileComponent(_tilePosition, this);
		}
	}

	std::wstring script = scriptFilename;
	script = script.append(L".json");
	_sprite = new Sprite;
	_sprite->Init(textureFilename, script);
}

void TrapItem::Deinit()
{

}
void TrapItem::Update(float deltaTime)
{
	_sprite->SetPosition(_position.x, _position.y);
	_sprite->Update(deltaTime);
}
void TrapItem::Render()
{
	_sprite->Render();
}

void TrapItem::Release()
{
	_sprite->Release();
}

void TrapItem::Reset()
{
	_sprite->Reset();
}

void TrapItem::ReceiveMsg(const sMessageParam& param)
{
	if (L"USE" == param.message)
	{
		sMessageParam msg;
		msg.sender = this;
		msg.receiver = param.sender;
		msg.message = L"AttakTrap";
		msg.attackPoint = 20;
		ComponentSystem::GetInstance()->SendMsg(msg);

		_isLive = false;
		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		if (NULL != map)
		{
			map->ResetTileComponent(_tilePosition, this);
		}
	}

}