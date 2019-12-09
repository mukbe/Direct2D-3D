#include "stdafx.h"
#include "Matrix2D.h"


ID2D1RenderTarget* Matrix2D::renderTarget = nullptr;

void Matrix2D::SetRenderTarget()
{
	if (renderTarget != nullptr)
	{
		LOG->Warning(__FILE__ , __LINE__, "Matrix2D Already Connected RenderTarget");
		return;
	}
	
	Log_WarnAssert(renderTarget = p2DRenderer->GetRenderTarget());
}

void Matrix2D::UpdateMatrix()
{
	//SRT
	result = scale * rotate * trans;
	rc = FloatRect(D3DXVECTOR2(0,0), size, pivot);
}

Matrix2D::Matrix2D()
	:pos(0, 0), size(0, 0), rotation(0.f), pivot(Pivot::LEFT_TOP)
{
	result = rotate = trans = scale = Matrix3x2F::Identity();
}

Matrix2D::Matrix2D(D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
{
	result = rotate = Matrix3x2F::Identity();
	trans = Matrix3x2F::Translation(pos.x, pos.y);
	scale = Matrix3x2F::Identity();

	this->pos = pos;
	this->size = size;
	pivot = p;

	UpdateMatrix();
}

void Matrix2D::SetScale(D3DXVECTOR2 s, D3DXVECTOR2 center)
{
	scale = Matrix3x2F::Scale(s.x, s.y, Point2F(center.x,center.y));
	UpdateMatrix();
	
}

void Matrix2D::SetScale(D3DXVECTOR2 s)
{
	scale = Matrix3x2F::Scale(s.x, s.y);
	UpdateMatrix();
}

void Matrix2D::SetScale(float x, float y)
{
	scale = Matrix3x2F::Scale(x, y);
	UpdateMatrix();

}

void Matrix2D::SetScale(float v)
{
	scale = Matrix3x2F::Scale(v,v);
	UpdateMatrix();
}

void Matrix2D::SetSize(D3DXVECTOR2 s)
{
	size = s;
	UpdateMatrix();
}

void Matrix2D::SetSize(float v)
{
	size = { v,v };
	UpdateMatrix();
}

void Matrix2D::SetSize(float x, float y)
{
	size = {x, y};
	UpdateMatrix();
}

void Matrix2D::SetRotate(float degree, D3DXVECTOR2 pos, bool isLocal, bool isDegree)
{
	rotation = degree;
	if (isDegree)
	{
		if (isLocal)
		{
			rotate = Matrix3x2F::Rotation(degree, Point2F(pos.x, pos.y));
		}
		else
		{
			rotate = Matrix3x2F::Rotation(degree);
		}
	}
	else
	{
		if (isLocal)
		{
			rotate = Matrix3x2F::Rotation(D3DXToDegree(degree), Point2F(pos.x, pos.y));
		}
		else
		{
			rotate = Matrix3x2F::Rotation(D3DXToDegree(degree));
		}
	}

	UpdateMatrix();
}

void Matrix2D::SetPos(D3DXVECTOR2 p)
{
	pos = p;
	trans = Matrix3x2F::Translation(p.x, p.y);
	UpdateMatrix();
}

void Matrix2D::SetPos(float x, float y)
{
	pos = { x,y };
	trans = Matrix3x2F::Translation(x, y);
	UpdateMatrix();
}

void Matrix2D::SetPos(POINT p)
{
	pos = { (float)p.x, (float)p.y };
	trans = Matrix3x2F::Translation(p.x, p.y);
	UpdateMatrix();
}

void Matrix2D::Bind()
{
	renderTarget->SetTransform(result);
}

void Matrix2D::Render()
{
	p2DRenderer->DrawRectangle(rc, DefaultBrush::blue);
}

//�׽�Ʈ ���غ�
Matrix2D  Matrix2D::operator*(const Matrix2D & other)
{
	Matrix2D val;
	val.scale = this->scale * other.scale;
	val.rotate = this->rotate * other.rotate;
	val.trans = this->trans * other.trans;
	val.result = this->result * other.result;
	val.pos = this->pos + other.pos;
	val.size.x = this->size.x * other.size.x;
	val.size.y = this->size.y * other.size.y;
	val.rotation = this->rotation + other.rotation;

	return val;
}


