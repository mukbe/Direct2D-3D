#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"


GameObject::GameObject(string name , D3DXVECTOR2 pos )
{
	//buffer = new PositionBuffer;
	worldBuffer = new WorldBuffer;
	shader = new Shader(L"./Shaders/Color.hlsl");

	transform = new Matrix2D();
	transform->SetPos(pos);
	
	rc.left = rc.top = -50;
	rc.right = rc.bottom = 50;
}


GameObject::~GameObject()
{
	Release();
	SafeDelete(transform);

	SafeDelete(shader);
	SafeDelete(worldBuffer);
	//SafeDelete(buffer);

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

void GameObject::PostRender()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	DeviceContext->IASetInputLayout(nullptr);
	DeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	shader->Render();

	//이거 수정해라
	CAMERA->CameraDataBind();
	worldBuffer->Setting(transform->GetResult());
	worldBuffer->SetPSBuffer(1);
	//buffer->SetPosition(D3DXVECTOR2(0,0));
	//buffer->SetVSBuffer(1);
	//buffer->SetPSBuffer(1);
	pRenderer->TurnOnAlphaBlend();
	DeviceContext->Draw(4, 0);
}
