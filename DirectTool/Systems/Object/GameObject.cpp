#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"
#include "Bounding.h"

GameObject::GameObject(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
	:defaultTexture(nullptr)
	, name(name), bActive(true), worldBuffer(nullptr), shader(nullptr)
	,  velocity(D3DXVECTOR2(0, 0)), accelerate(D3DXVECTOR2(0, 0))
	, alpha(1.f), size(size),  lifeTiem(0.f), frameTime(0.f)
{

	worldBuffer = new WorldBuffer;
	shader = new Shader(L"./Shaders/Color.hlsl");

	transform = new Matrix2D(pos, size, p);

	bActive = true;

	frameX = frameY = 0;

	bound = new BoundingBox(this);
	frequency = 0.07f;
	gravity = D3DXVECTOR2(0, 0.f);
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

	if (Keyboard::Get()->Press('G'))
		size += D3DXVECTOR2(50, 50) * Time::Delta();
	else if (Keyboard::Get()->Press('F'))
		size += D3DXVECTOR2(-50, -50) * Time::Delta();







	lifeTiem += tick;
	frameTime += tick;

	//애니메이션으로 바꿔야됨
	if (sprites.size() != 0)
	{
		if (frameTime >= frequency)
		{
			frameX++;
			frameTime -= frequency;
			if (frameX >= sprites[state]->GetMaxFrame().x)
			{
				frameY++;
				frameX = 0;


				if (frameY >= sprites[state]->GetMaxFrame().y)
				{
					frameY = 0;
				}
			}
		}

	}

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
	if (defaultTexture != nullptr)
	{
		//defaultTexture->Render();
		//defaultTexture->FrameRender(frameX, frameY, size, 1.f, pivot);
	}
	bound->Render();
	//p2DRenderer->FillEllipse(rc.GetRect());
	
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

void GameObject::SetTextureFilePath(wstring file)
{
	//TODO 리소스관리에서 확인후 있으면 포인터 반환 없으면 생성해서 반환

}

void GameObject::SetTexture(Texture * tex)
{
	if (defaultTexture != nullptr)
	{
		LOG->Warning(__FILE__, __LINE__, "Texture Confirm");
	}
	
	defaultTexture = tex;


}

void GameObject::SetSprite(State state, Texture * tex)
{
	if (sprites[state] != nullptr)
		LOG->Warning(__FILE__, __LINE__, "Sprite Confirm");

	sprites[state] = tex;
	

}
