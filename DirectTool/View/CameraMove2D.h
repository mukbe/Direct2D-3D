#pragma once
#include "./View/CameraBase.h"

class CameraMove2D : public CameraBase
{
	Synthesize(float, nowSpeed, NowSpeed)
	Synthesize(float, maxSpeed, MaxSpeed)

public:
	CameraMove2D();
	~CameraMove2D();

	virtual void Init();
	virtual void Releaes();
	virtual void Update();
	virtual void Render();
	virtual void UIRender();
private:
	void ControlCamera();
	void UpdateView();

};

