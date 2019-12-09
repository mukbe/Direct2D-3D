#pragma once

#define ZOOM_MAX 2.5f
#define ZOOM_MIN 0.5f

//마우스의 휠메세지 받아와서 줌 크기 결정하면 될듯
class CameraManager
{
	SingletonHeader(CameraManager)
public:

	void Update();
	void ImguiRender();

	RECT GetRenderRect() { return renderRect; }
	void UpdateRenderRect();

	D3DXVECTOR2 GetPos() { return pos; }
	void SetPos(D3DXVECTOR2 p)
	{
		pos = p;
		UpdateRenderRect();
	}

	float GetZoom() { return zoom; }
	void AddZoom(float value);

	RECT GetRelativeRECT(RECT rc);
	D3DXVECTOR2 ScreenToWorld(D3DXVECTOR2 vr);
	D3DXVECTOR2 WorldToScreen(D3DXVECTOR2 vr);

	D3DXVECTOR2 GetMousePos();

	BOOL IsCollision(D3DXVECTOR2 p);
	Matrix2D GetView() { return view; }

	void CameraDataBind();
private:
	void UpdateMatrix();
	void ClipMouse();
	
	Matrix2D view;
	D3DXVECTOR2 pos;
	RECT renderRect;
	float zoom;
	
	D3DXVECTOR2 pick;
	D3DXVECTOR2 picked;


	//카메라 cbuffer 필요


private:

	class CameraBuffer : public ShaderBuffer
	{
	private:
		struct Struct
		{

			D3DXVECTOR4 Matrix;

			D3DXVECTOR2 CameraPos;
			D3DXVECTOR2 Padding;

			

		}data;

	public:
		CameraBuffer()
			:ShaderBuffer(&data, sizeof Struct)
		{}

		void Setting(D2D1::Matrix3x2F mat)
		{
			data.CameraPos = { mat._31,mat._32 };
			memcpy(&data.Matrix, &mat, sizeof(FLOAT)*4);
		}


	};

	CameraBuffer* buffer;


};

#define CAMERA CameraManager::Get()