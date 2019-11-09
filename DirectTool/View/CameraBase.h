#pragma once

class OrthoWindow;
class CameraBase 
{
protected:
	Synthesize(OrthoWindow*, ortho, Ortho)
	Synthesize(D3DXMATRIX, matView, ViewMatrix)
public:
	CameraBase();
	virtual ~CameraBase();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual D3DXMATRIX GetProjection();
protected:
	D3DXVECTOR3 pos;

};
