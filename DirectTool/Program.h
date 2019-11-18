#pragma once

class SceneMain;
class Program
{



public:
	Program();
	~Program();

	void PreUpdate();
	void Update(float tick);
	void PostUpdate();

	void Render();

	void PostRender();
	void ImguiRender();

	void ResizeScreen();
private:
	//class Json::Value* jsonRoot;
	//class DeferredRenderer* deferred;
	//class ShadowRenderer* shadow;
	class SceneMain *sm;

	ID3D11Buffer* vertexBuffer, *indexBuffer;

	FloatRect play;

	GameObject* gameObject;
	GameObject* gameObject2;
	Texture* tex;
};