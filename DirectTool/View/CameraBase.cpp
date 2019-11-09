#include "stdafx.h"
#include "CameraBase.h"
#include "../Render/WorldBuffer.h"
#include "./View/OrthoWindow.h"



CameraBase::CameraBase()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);

	//this->ortho = new OrthoWindow(D3DXVECTOR2(-WinSizeX / 2, WinSizeY / 2), D3DXVECTOR2(WinSizeX, WinSizeY));

	this->ortho = new OrthoWindow(D3DXVECTOR2(-WinSizeX / 2, WinSizeY / 2), D3DXVECTOR2(WinSizeX/4, WinSizeY/4));


	pos = D3DXVECTOR3(0, 0, -20);

	D3DXMatrixLookAtLH(&matView, &pos, &(pos + D3DXVECTOR3(0.f, 0.f, 1.f)), &D3DXVECTOR3(0.f, 1.f, 0.f));

}


CameraBase::~CameraBase()
{
}

void CameraBase::Init()
{
}

void CameraBase::Release()
{
}

void CameraBase::Update()
{


}

void CameraBase::Render()
{
	ortho->Render();
}

D3DXMATRIX CameraBase::GetProjection()
{
	return ortho->GetMatrix();
}

