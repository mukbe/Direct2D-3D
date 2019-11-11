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
	renderTarget = p2DRenderer->GetRenderTarget();
}

void Matrix2D::UpdateMatrix()
{
	//SRT
	result = scale * rotate * trans;
}

Matrix2D::Matrix2D()
{
	result = rotate = trans = scale = Matrix3x2F::Identity();
}

void Matrix2D::SetScale(D3DXVECTOR2 s, D3DXVECTOR2 center)
{
	scale = Matrix3x2F::Scale(s.x, s.y,Point2F(center.x,center.y));
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

void Matrix2D::SetRotate(float degree, D3DXVECTOR2 pos, bool isLocal, bool isDegree)
{
	if (isDegree)
	{
		if (isLocal)
		{
			if (pos.x == 0 && pos.y == 0)
			{
				//LOG->Warning(__FILE__, __LINE__, "Is not Local pos");
			}
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
	trans = Matrix3x2F::Translation(p.x, p.y);
	UpdateMatrix();
}

void Matrix2D::SetPos(float x, float y)
{
	trans = Matrix3x2F::Translation(x, y);
	UpdateMatrix();
}

void Matrix2D::SetPos(POINT p)
{
	trans = Matrix3x2F::Translation(p.x, p.y);
	UpdateMatrix();
}

void Matrix2D::Bind()
{
	renderTarget->SetTransform(result);
}


