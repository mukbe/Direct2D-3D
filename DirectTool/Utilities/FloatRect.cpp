#include "stdafx.h"
#include "FloatRect.h"



FloatRect FloatRect::MoveRect( FloatRect  rc, float dx, float dy)
{
	FloatRect r = rc;
	r.left += dx;
	r.right += dx;
	r.top += dy;
	r.bottom == dy;
	return std::move(r);
}

FloatRect FloatRect::MoveRect(FloatRect rc, D3DXVECTOR2 v)
{
	FloatRect r = rc;
	r.left += v.x;
	r.right += v.x;
	r.top += v.y;
	r.bottom == v.y;
	return std::move(r);
}

FloatRect FloatRect::operator+=(D3DXVECTOR2  v)
{
	left += v.x;
	right += v.x;
	top += v.y;
	bottom += v.y;
	return *this;
}

FloatRect FloatRect::operator-=(D3DXVECTOR2 v)
{
	left -= v.x;
	right -= v.x;
	top -= v.y;
	bottom -= v.y;
	return *this;
}

FloatRect::FloatRect()
	:left(0),top(0),right(0),bottom(0)
{
}

FloatRect::FloatRect(D3DXVECTOR2 pos, float d, bool IsCenter)
{
	if (IsCenter)
	{
		left = pos.x - d/2.f;
		right = pos.x + d/2.f;
		top = pos.y - d / 2.f;
		bottom = pos.y + d / 2.f;
	}
	else
	{
		left = pos.x;
		right = pos.x + d;
		top = pos.y;
		bottom = pos.y + d;
	}
}

FloatRect::FloatRect(D3DXVECTOR2 pos, float w, float h, bool IsCenter)
{
	if (IsCenter)
	{
		left = pos.x - w / 2.f;
		right = pos.x + w / 2.f;
		top = pos.y - h / 2.f;
		bottom = pos.y + h / 2.f;
	}
	else
	{
		left = pos.x;
		right = pos.x + w;
		top = pos.y;
		bottom = pos.y + h;
	}
}

FloatRect::FloatRect(float l, float t, float r, float b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

RECT FloatRect::GetRect()
{
	RECT rc;
	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;
	return std::move(rc);
}


