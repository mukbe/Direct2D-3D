#pragma once
class Math
{
public:
	static const float PI;
	static const float Epsilon;

	static const int IntMin;
	static const int IntMax;
	static const float FloatMin;
	static const float FloatMax;

	static float NegativeChecking(float value);
	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	// Returns random float in [0, 1).
	static float RandF();

	static float Lerp(float val1, float val2,float amount);
	static float Clamp(float value, float min, float max);

	static float Modulo(float val1, float val2);

	static D3DXCOLOR RandomColor();

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}

	template<typename T>
	static T Abs(const T& a)
	{
		if (a >= 0.0f)
			return a;
		else
			return -a;
	}
	static bool FloatEqual(const float & f1, const float & f2)
	{
		return (fabsf(f1 - f2) <= Math::Epsilon);
	}


	static float Angle(D3DXVECTOR3 v1, D3DXVECTOR3 v2);

	static bool IsPointInAABB(D3DXVECTOR2 rectPos, D3DXVECTOR2 rectSize, D3DXVECTOR2 ptPos);
	static bool IsPointInAABB(FloatRect rc, D3DXVECTOR2 ptPos);
	static bool IsPointInCircle(D3DXVECTOR2 origin, float radius, D3DXVECTOR2 ptPos);

	static bool IsAABBInAABB(FloatRect rc0, FloatRect rc1, FloatRect * temp = nullptr);
	static bool IsAABBInCircle(FloatRect rc, D3DXVECTOR2 origin, float radius);
	static bool IsAABBInAABBReaction(FloatRect* me, FloatRect other);

};