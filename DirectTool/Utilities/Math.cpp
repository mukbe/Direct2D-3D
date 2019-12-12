#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;
const float Math::Epsilon = 1E-6f;
const int Math::IntMin = -2147483647;
const int Math::IntMax = 2147483647;
const float Math::FloatMin = -3.402823E+38f;
const float Math::FloatMax = 3.402823E+38f;
const D3DXVECTOR2 Math::gravity = { 0, 9.8f * 5.0f };
float Math::NegativeChecking(float value)
{
	if (value < 0.0f)
		return -1.0f;
	else
		return 1.0f;
}

float Math::ToRadian(float degree)
{
	return degree * PI / 180.0f;
}

float Math::ToDegree(float radian)
{
	return radian * 180.0f / PI;
}

float Math::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;
	
	return r1 + val;
}

float Math::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

float Math::Lerp(float val1, float val2, float amount)
{
	return val1 + (val2 - val1)* amount;
}

float Math::Clamp(float value, float min, float max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}


float Math::Modulo(float val1, float val2)
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}

D3DXCOLOR Math::RandomColor()
{
	float r = Math::Random(0.0f, 1.0f);
	float g = Math::Random(0.f, 1.0f);
	float b = Math::Random(0.f, 1.0f);
	float a = Math::Random(0.3f, 0.5f);
	return D3DXCOLOR(r,g,b,a);
}


int Math::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}


float Math::Angle(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float angle = 0;

	float dot = D3DXVec3Dot(&v1, &v2);
	float length1 = D3DXVec3Length(&v1);
	float length2 = D3DXVec3Length(&v2);

	angle = acosf(dot / (length1 * length2));

	return angle;
}


bool Math::IsPointInAABB(D3DXVECTOR2 rectPos, D3DXVECTOR2 rectSize, D3DXVECTOR2 ptPos)
{
	float left, top, right, bottom;
	left = rectPos.x;
	top = rectPos.y;
	right = rectPos.x + rectSize.x;
	bottom = rectPos.y - rectSize.y;

	if (ptPos.x < left || ptPos.x > right)
		return false;
	else if (ptPos.y < bottom || ptPos.y > top)
		return false;

	return true;
}

bool Math::IsPointInAABB(FloatRect rc, D3DXVECTOR2 ptPos)
{
	if (ptPos.x < rc.left || ptPos.x > rc.right)
		return false;
	else if (ptPos.y < rc.bottom || ptPos.y > rc.top)
		return false;

	return true;
}

bool Math::IsPointInCircle(D3DXVECTOR2 origin, float radius, D3DXVECTOR2 ptPos)
{
	float distance = D3DXVec2Length(&(ptPos - origin));

	if (distance <= radius) return true;

	return false;
}

bool Math::IsAABBInAABB(FloatRect rc0, FloatRect rc1, FloatRect * temp)
{
	if (rc0.right < rc1.left || rc0.left > rc1.right) return false;
	if (rc0.bottom < rc1.top || rc0.top > rc1.bottom) return false;

	if (temp != nullptr)
	{
		temp->left = Math::Max(rc0.left, rc1.left);
		temp->right = Math::Min(rc0.right, rc1.right);
		temp->top = Math::Max(rc0.top, rc1.top);
		temp->bottom = Math::Min(rc0.bottom, rc1.bottom);
	}
	return true;
}

bool Math::IsAABBInCircle(FloatRect rc, D3DXVECTOR2 origin, float radius)
{
	FloatRect temp, pt;
	pt = FloatRect(origin, radius, Pivot::CENTER);
	if (IsAABBInAABB(rc, pt))
	{
		if (origin.x <= rc.left && origin.y <= rc.top)
		{
			float dx = fabsf(rc.left - origin.x);
			float dy = fabsf(rc.top - origin.y);
			if (dx * dx + dy * dy > radius * radius)
				return false;
		}
		if (origin.x >= rc.right && origin.y <= rc.top)
		{
			float dx = fabsf(rc.right - origin.x);
			float dy = fabsf(rc.top - origin.y);
			if (dx * dx + dy * dy > radius * radius)
				return false;
		}
		if (origin.x >= rc.right && origin.y >= rc.bottom)
		{
			float dx = fabsf(rc.right - origin.x);
			float dy = fabsf(rc.bottom - origin.y);
			if (dx * dx + dy * dy > radius * radius)
				return false;
		}
		if (origin.x <= rc.left && origin.y >= rc.bottom)
		{
			float dx = fabsf(rc.left - origin.x);
			float dy = fabsf(rc.bottom - origin.y);
			if (dx * dx + dy * dy > radius * radius)
				return false;
		}

		return true;
	}

	return false;
}

bool Math::IsAABBInAABBReaction(FloatRect * me, FloatRect other, D3DXVECTOR2* dir)
{
	FloatRect rcInter;

	if (IsAABBInAABB(*me, other, &rcInter) == false)
		return false;

	float interW = rcInter.right - rcInter.left;
	float interH = rcInter.bottom - rcInter.top;

	if (Math::FloatEqual(interW, interH))
	{
		if (Math::FloatEqual(rcInter.left, other.left))
		{
			me->left -= interW;
			me->right -= interW;
			if (dir)
				*dir = D3DXVECTOR2(1.f, 0.f);
		}
		else if (Math::FloatEqual(rcInter.right, other.right))
		{
			me->left += interW;
			me->right += interW;
			if (dir)
				*dir = D3DXVECTOR2(-1.f, 0.f);
		}
		//위
		if (Math::FloatEqual(rcInter.top, other.top))
		{
			me->top -= interH;
			me->bottom -= interH;
			if (dir)
				*dir = D3DXVECTOR2(0.f, -1.f);
		}
		//아래
		else if (Math::FloatEqual(rcInter.bottom, other.bottom))
		{
			me->top += interH;
			me->bottom += interH;
			if (dir)
				*dir = D3DXVECTOR2(0.f, 1.f);
		}
	}
	else if (interW < interH)
	{
		if (Math::FloatEqual(rcInter.left, other.left))
		{
			me->left -= interW;
			me->right -= interW;
			if (dir)
				*dir = D3DXVECTOR2(1.f, 0.f);
		}
		else if (Math::FloatEqual(rcInter.right, other.right))
		{
			me->left += interW;
			me->right += interW;
			if (dir)
				*dir = D3DXVECTOR2(-1.f, 0.f);
		}
	}
	else
	{
		//위
		if (Math::FloatEqual(rcInter.top, other.top))
		{
			me->top -= interH;
			me->bottom -= interH;
			if (dir)
				*dir = D3DXVECTOR2(0.f, 1.f);
		}
		//아래
		else if (Math::FloatEqual(rcInter.bottom, other.bottom))
		{
			me->top += interH;
			me->bottom += interH;
			if (dir)
				*dir = D3DXVECTOR2(0.f, -1.f);
		}
	}

	return true;
	
}
