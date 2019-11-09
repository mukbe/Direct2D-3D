#pragma once

class SceneNode
{
public:
	SceneNode() {}
	virtual ~SceneNode() {}

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void PreUpdate() {}
	virtual void Update(float tick) = 0;
	virtual void PostUpdate() {}

	virtual void Render() = 0;
	virtual void ImguiRender() {}
};

