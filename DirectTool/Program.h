#pragma once

class SceneMain;
class ObjectManager;
class Player;
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


	GameObject* gameObject;
	GameObject* gameObject2;
	Texture* tex;

	vector<GameObject*> objs;

	ObjectManager* objManager;



	Player* player;
};