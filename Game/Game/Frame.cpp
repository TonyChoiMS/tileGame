#include "Frame.h"
#include "Texture.h"
<<<<<<< HEAD
=======
#include "GameSystem.h"
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

Frame::Frame()
{
	_frameTime = 0.0f;
}

Frame::~Frame()
{

}

<<<<<<< HEAD
void Frame::Init(ID3DXSprite* spriteDX, Texture* texture, int x, int y, int width, int height, float frameTime)
{
	_spriteDX = spriteDX;
	//_textureDX = textureDX;
=======
void Frame::Init(Texture* texture, int x, int y, int width, int height, float frameTime)
{
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	_texture = texture;

	// ȭ�鿡 ��µ� �̹����� ������ ����
	_textureRect.left = x;
	_textureRect.right = _textureRect.left + width;
	_textureRect.top = y;
	_textureRect.bottom = _textureRect.top + height;
	// �ؽ��Ŀ� ����� �÷�( ���͸��� ���� )
	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);			// ������� �ϸ� �����̹��� �״�� ���

	_frameTime = frameTime;

	_width = width;
	_height = height;
}

void Frame::Render()
{
	// ũ�� ����
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);

	// ȸ��
	D3DXVECTOR2 center = D3DXVECTOR2(_width / 2.0f, _height / 2.0f);
	float rotate = 0.0f;

	// ��ġ ����
<<<<<<< HEAD
	float x = 1280.0f / 2.0f - _width / 2.0f;
	float y = 800.0f / 2.0f - _height / 2.0f;
=======
	float x = _x - _width / 2.0f;
	float y = _y - _height / 2.0f;
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	D3DXVECTOR2 translate = D3DXVECTOR2(x, y);

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,				// ������ Matrix( �������� ����� ��)
		NULL,					// Scale�� �߽���
		0.0f,					// Ȯ�� ȸ�� ���.
		&scale,					// Ȯ��. ũ�� ����
		&center,				// ȸ���߽�
		rotate,					// ȸ�� ��
		&translate				// ���� �̵� -> ��ġ�� ����
	);

<<<<<<< HEAD
	_spriteDX->SetTransform(&matrix);
	//_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	_spriteDX->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);
=======
	GameSystem::GetInstance()->GetSpriteDX()->SetTransform(&matrix);
	GameSystem::GetInstance()->GetSpriteDX()->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
}

void Frame::Release()
{

}

//void Frame::Reset(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX)
void Frame::Reset(ID3DXSprite* spriteDX)
{
<<<<<<< HEAD
	_spriteDX = spriteDX;
	//_textureDX = textureDX;
=======
}

void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
}
