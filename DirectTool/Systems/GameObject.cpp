#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"


GameObject::GameObject(string name , D3DXVECTOR2 pos )
	:defaultTexture(nullptr)
{
	worldBuffer = new WorldBuffer;
	shader = new Shader(L"./Shaders/Color.hlsl");

	transform = new Matrix2D();
	transform->SetPos(pos);
	
	rc.left = rc.top = -50;
	rc.right = rc.bottom = 50;
	size.x = size.y = 100;
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

void GameObject::Update()
{
	if (Keyboard::Get()->Press('G'))
		size += D3DXVECTOR2(10, 10) * Time::Delta();
	if (Keyboard::Get()->Press('F'))
		size += D3DXVECTOR2(-10, -10) * Time::Delta();

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

	if(defaultTexture != nullptr)
		defaultTexture->Render(size);

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
