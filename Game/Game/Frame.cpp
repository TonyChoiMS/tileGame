#include "Frame.h"
#include "GameSystem.h"
#include "Texture.h"
Frame::Frame()
{
	_frameTime = 0.3f;
	_rotate = 0.0f;
}
Frame::~Frame()
{

}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameTime)
{
	//_spriteDX = spriteDX;
	//textureDX = textureDX;
	_texture = texture;
	_textureRect.left = x;									//
	_textureRect.right = _textureRect.left + width;	//
	_textureRect.top = y;									//
	_textureRect.bottom = _textureRect.top + height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);		// �̹��� ���� (���� ����̸�, ���� �״�� ����)
	_rotate = rotate;
	_width = width;
	_height = height;
	_frameTime = frameTime;
}
void Frame::Render()
{
	//_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	// ũ�� ����
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);
	// ȸ��
	D3DXVECTOR2 center = D3DXVECTOR2(_width / 2.0f, _height / 2.0f);
	//float roate =0.0f;
	float roate = _rotate;
	// ��ġ ����
	//float x = 1280.0f / 2.0f - _width / 2.0f;
	//float y = 800.0f / 2.0f - _height / 2.0f;

	float x = _x - _width / 2.0f;
	float y = _y - _height / 2.0f;
	D3DXVECTOR2 translate = D3DXVECTOR2(x, y);


	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,					// ���� ���
		NULL,						// �������� �߽���
		0.0f,						// Ȯ�� ȸ�� ���. 
		&scale,						// ������ ũ��(Ȯ�� ,ũ�����)
		&center,					// ȸ�� �߽�
		roate,						// ȸ����,
		&translate					// ���� �̵�-> ��ġ�� ����
	);

	GameSystem::GetInstance()->GetSpriteDX()->SetTransform(&matrix);
	//_spriteDX->SetTransform(&matrix);
	GameSystem::GetInstance()->GetSpriteDX()->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);

	//_spriteDX->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);

}
void Frame::Relase()
{

}

void Frame::Reset()
{
	//_spriteDX = spriteDX;
	//_textureDX = textureDX;
}
void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}


float Frame::GetFrameTime()
{
	//GameTimer time;
	return _frameTime;
}