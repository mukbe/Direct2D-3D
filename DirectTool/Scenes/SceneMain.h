#pragma once
#include "SceneNode.h"
class SceneMain : public SceneNode
{
public:
	SceneMain();
	virtual ~SceneMain();

	virtual void Init();
	virtual void Release();
	
	virtual void PreUpdate();
	virtual void Update(float tick);
	virtual void PostUpdate();

	virtual void Render();
	virtual void ImguiRender();
};

