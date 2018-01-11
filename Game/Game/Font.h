#pragma once

#include <d3dx9.h>
#include <string>

class Font
{
private:
	ID3DXFont* _fontDX;
	RECT _rect;
	D3DCOLOR _color;
	std::wstring _text;
	std::wstring _fontName;
	int _fontSize;
	int _width;
	int _height;

public:
	Font(std::wstring fontName, int fontSize, D3DCOLOR fontColor);
	~Font();

	void Render();

	void Release();
	void Reset();

	void SetPosition(int x, int y);
	void SetRect(int x, int y, int width, int height);
	void SetText(std::wstring text);
};