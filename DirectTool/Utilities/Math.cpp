#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;
const float Math::Epsilon = 1E-6f;
const int Math::IntMin = -2147483647;
const int Math::IntMax = 2147483647;
const float Math::FloatMin = -3.402823E+38f;
const float Math::FloatMax = 3.402823E+38f;

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
