#pragma once

class FloatRect
{
public:
	static FloatRect MoveRect(FloatRect rc, float dx, float dy);
	static FloatRect MoveRect(FloatRect rc, D3DXVECTOR2 v);

	FloatRect operator+=(const D3DXVECTOR2& v);
	FloatRect operator-=(const D3DXVECTOR2& v);
public:
	float left;
	float top;
	float right;
	float bottom;

	FloatRect();
	FloatRect(D3DXVECTOR2 pos, float d , bool IsCenter = false);
	FloatRect(D3DXVECTOR2 pos, float w, float h, bool IsCenter = false);
	FloatRect(float l, float t, float r, float b);
	RECT GetRect();

};

