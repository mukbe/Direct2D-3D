#pragma once

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

private:
	unordered_map<string, GameObject*> objects;
};


