#include "stdafx.h"
#include "Bounding.h"



Bounding::Bounding(Figure f)
	:figure(f),offset(offset)
{
}


Bounding::~Bounding()
{
}

void Bounding::SetGameObject(GameObject * obj, D3DXVECTOR2 offset)
{
	this->obj = obj;
	this->offset = offset;
	renderRc = FloatRect(this->offset, obj->GetSize(), obj->GetPivot());

	Matrix2D* mat = obj->Transform();
	rc = renderRc;
	rc += D3DXVECTOR2(mat->GetResult().dx, mat->GetResult().dy);
}

//회전은 하지않을것
void Bounding::Update()
{
	Matrix2D* mat = obj->Transform();
	rc = renderRc;
	rc += D3DXVECTOR2(mat->GetResult().dx, mat->GetResult().dy);
	//rc = FloatRect(  renderRc + D3DXVECTOR2(mat->GetResult().dx, mat->GetResult().dy);

}

void Bounding::Render()
{
	switch (figure)
	{
	case Figure::Box :
		p2DRenderer->DrawRectangle(renderRc.GetRect());
		break;
	case Figure::Circle:
		p2DRenderer->DrawEllipse(renderRc.GetRect());
		break;

	}
}

void Bounding::SetOffset(D3DXVECTOR2 offset)
{
	this->offset = offset;
	renderRc = FloatRect(offset, obj->GetSize(), obj->GetPivot());
}
//절대좌표에서의 위치를 반환한다
FloatRect Bounding::GetRect()
{
	Matrix2D* mat = obj->Transform();
	rc = renderRc;
	rc += D3DXVECTOR2(mat->GetResult().dx, mat->GetResult().dy);
	return rc;
}
