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
	rc = FloatRect(this->offset, obj->GetSize(), obj->GetPivot());
}

void Bounding::Update()
{
	rc = FloatRect(offset, obj->GetSize(), obj->GetPivot());
}

void Bounding::Render()
{
	switch (figure)
	{
	case Figure::Box :
		p2DRenderer->DrawRectangle(rc.GetRect());
		break;
	case Figure::Circle:
		p2DRenderer->DrawEllipse(rc.GetRect());
		break;

	}
}
