#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"
#include "Bounding.h"

GameObject::GameObject(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, ObjectType type ,Pivot p)
	:defaultTexture(nullptr)
	, name(name), bActive(true), worldBuffer(nullptr), shader(nullptr)
	, alpha(1.f), size(size),  lifeTiem(0.f), frameTime(0.f), objectType(type)
{
	worldBuffer = new WorldBuffer;
	shader = new Shader(L"./Shaders/Color.hlsl");
	transform = new Matrix2D(pos, size, p);

	bActive = true;

	frameX = frameY = 0;

	bound = new BoundingBox(this);
	frequency = 0.07f;
}


GameObject::~GameObject()
{
	Release();
	SafeDelete(transform);
	SafeDelete(shader);
	SafeDelete(worldBuffer);
}

void GameObject::Init()
{
	
}

void GameObject::Release()
{
}

void GameObject::PreUpdate()
{
	bound->Update();
}

void GameObject::Update(float tick)
{

	if (Keyboard::Get()->Down(VK_F5))
		bActive = !bActive;

	if (bActive == false) return;


	lifeTiem += tick;
	frameTime += tick;

	////애니메이션으로 바꿔야됨
	//if (sprites.size() != 0)
	//{
	//	if (frameTime >= frequency)
	//	{
	//		frameX++;
	//		frameTime -= frequency;
	//		if (frameX >= sprites[state]->GetMaxFrame().x)
	//		{
	//			frameY++;
	//			frameX = 0;


	//			if (frameY >= sprites[state]->GetMaxFrame().y)
	//			{
	//				frameY = 0;
	//			}
	//		}
	//	}

	//}

}

void GameObject::PostUpdate()
{
}

//뷰행렬 계산을 하면 TRUE
void GameObject::Render(bool isRelative)
{
	if (bActive == false)return;

	Matrix2D world = *transform;

	if (isRelative)
	{
		world = world * CAMERA->GetView();
	}

	world.Bind();

	transform->Render();
	bound->Render();

	if (defaultTexture != nullptr)
	{
		//defaultTexture->Render();
		//defaultTexture->FrameRender(frameX, frameY, size, 1.f, pivot);
	}
	
}

void GameObject::PostRender()
{

	CAMERA->CameraDataBind();
	worldBuffer->Setting(transform->GetResult());
	worldBuffer->SetPSBuffer(1);
	shader->Render();

	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	DeviceContext->IASetInputLayout(nullptr);
	DeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pRenderer->TurnOnAlphaBlend();
	DeviceContext->Draw(4, 0);
	pRenderer->TurnOffAlphaBlend();

}
