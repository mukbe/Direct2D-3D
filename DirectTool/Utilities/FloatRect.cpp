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

FloatRect FloatRect::operator+=(const D3DXVECTOR2&  v)
{
	left += v.x;
	right += v.x;
	top += v.y;
	bottom += v.y;
	return *this;
}

FloatRect FloatRect::operator-=(const D3DXVECTOR2& v)
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

FloatRect::FloatRect(D3DXVECTOR2 pos, float d, Pivot p)
{
	Calculate(pos, D3DXVECTOR2(d, d), p);
}

FloatRect::FloatRect(D3DXVECTOR2 pos, float w, float h, Pivot p)
{
	Calculate(pos, D3DXVECTOR2(w, h), p);
}

FloatRect::FloatRect(D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
{
	Calculate(pos, size, p);
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

void FloatRect::Calculate(D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
{
	switch (p)
	{
	case LEFT_TOP:
		left = pos.x;
		right = pos.x + size.x;
		top = pos.y;
		bottom = pos.y + size.y;
		break;
	case CENTER:
		left = pos.x - size.x / 2.f;
		right = pos.x + size.x / 2.f;
		top = pos.y - size.y / 2.f;
		bottom = pos.y + size.y / 2.f;
		break;
	case BOTTOM:
		left = pos.x - size.x / 2.f;
		right = pos.x + size.x / 2.f;
		top = pos.y - size.y;
		bottom = pos.y;
		break;
	case TOP:
		left = pos.x - size.x / 2.f;
		right = pos.x + size.x / 2.f;
		top = pos.y;
		bottom = pos.y + size.y;
		break;
	}

}


