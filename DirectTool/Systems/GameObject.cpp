#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"


GameObject::GameObject(string name , D3DXVECTOR2 pos )
{
	transform = new Matrix2D();
	transform->SetPos(pos);
	
	rc.left = rc.top = -50;
	rc.right = rc.bottom = 50;
}


GameObject::~GameObject()
{
	Release();
	SafeDelete(transform);
}

void GameObject::Init()
{
}

void GameObject::Release()
{
}

void GameObject::Update()
{
}

//뷰행렬 계산을 하면 TRUE
void GameObject::Render(bool isRelative)
{
	Matrix2D world;

	if (isRelative)
	{
		world = *transform * CAMERA->GetView();
	}

	world.Bind();

	p2DRenderer->FillEllipse(rc.GetRect());

}
