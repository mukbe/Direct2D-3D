#pragma once

using namespace D2D1;

class Matrix2D
{
private:
	static 	ID2D1RenderTarget* renderTarget;

public:
	static void SetRenderTarget();

private:
	Matrix3x2F rotate;
	Matrix3x2F scale;
	Matrix3x2F trans;
	Matrix3x2F result;

	void UpdateMatrix();

public:
	Matrix2D();

	Matrix3x2F GetResult() { return result; }

	void SetScale(D3DXVECTOR2 s, D3DXVECTOR2 center);
	void SetScale(D3DXVECTOR2 s);
	void SetScale(float x, float y);
	void SetScale(float v);

	void SetRotate(float degree, D3DXVECTOR2 pos = D3DXVECTOR2(0,0), bool isLocal = true, bool isDegree = true);

	void SetPos(D3DXVECTOR2 p);
	void SetPos(float x, float y);
	void SetPos(POINT p);

	void Bind();


	Matrix2D operator*(const Matrix2D& other);
	
};

