#pragma once

class PhysicsWorld;
class GameObject;
class Shape;
class RigidBody;
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void PreUpdate();
	void Update(float tick);
	void PostUpdate();

	void Render();

	void PostRender();
	void ImguiRender();


	void AddObject(GameObject* object);
private:
	unordered_map<string, GameObject*> objects;
	PhysicsWorld* world;
};


