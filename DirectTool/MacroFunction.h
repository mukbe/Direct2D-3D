#pragma once

//RECT
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };

	return rc;
}

inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };

	return rc;
}

inline float Clamp(float val, float s, float big)
{
	return max(s, min(big, val));
}
inline bool EllipseRectCollision(D3DXVECTOR2 origin, float r, RECT rc)
{
	RECT temp, pt;
	pt = RectMakeCenter((int)origin.x, (int)origin.y, (int)r, (int)r);
	if (IntersectRect(&temp, &rc, &pt))
	{
		if (origin.x <= rc.left && origin.y <= rc.top)
		{
			float dx = fabsf(rc.left - origin.x);
			float dy = fabsf(rc.top - origin.y);
			if (dx*dx + dy * dy > r*r)
				return false;
		}
		if (origin.x >= rc.right && origin.y <= rc.top)
		{
			float dx = fabsf(rc.right - origin.x);
			float dy = fabsf(rc.top - origin.y);
			if (dx*dx + dy * dy > r*r)
				return false;
		}
		if (origin.x >= rc.right && origin.y >= rc.bottom)
		{
			float dx = fabsf(rc.right - origin.x);
			float dy = fabsf(rc.bottom - origin.y);
			if (dx*dx + dy * dy > r*r)
				return false;
		}
		if (origin.x <= rc.left && origin.y >= rc.bottom)
		{
			float dx = fabsf(rc.left - origin.x);
			float dy = fabsf(rc.bottom - origin.y);
			if (dx*dx + dy * dy > r*r)
				return false;
		}

		return true;
	}

	return false;
}


inline void MoveRect(RECT* rc, int dx, int dy)
{
	rc->left += dx;
	rc->right += dx;
	rc->top += dy;
	rc->bottom += dy;
}