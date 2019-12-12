#pragma once
class RigidBody;
class Manifold;
class Shape;
class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void Update();
	void Render();
	


	RigidBody* Add(GameObject* obj,RigidType type, Shape* shape);
	std::vector<RigidBody *> GetBody() { return bodies; }


	bool bActive;

private:
	
	int m_iterations;
	std::vector<RigidBody *> bodies;
	std::vector<Manifold> contacts;


	void IntegrateForces(RigidBody *b);
	void IntegrateVelocity(RigidBody *b);


};

