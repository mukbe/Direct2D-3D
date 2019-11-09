#pragma once

using namespace D2D1;

class Matrix2D
{
private:
	Matrix3x2F rotate;
	Matrix3x2F scale;
	Matrix3x2F trans;
	Matrix3x2F result;
public:
	Matrix2D();

	void SetScale(D3DXVECTOR2 s);
	void SetScale(float x, float y);

	void SetRotate(float degree , bool isDegree = true);

	void SetPos(D3DXVECTOR2 p);
	void SetPos(float x, float y);
	void SetPos(POINT p);

};

