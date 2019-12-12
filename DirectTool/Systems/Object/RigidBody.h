#pragma once

class GameObject;
class Matrix2D;
struct Shape;
class RigidBody
{
	friend class ObejctManger;
	friend struct Shape;
	friend class Circle;
	friend class PolygonShape;
	friend struct Manifold;
	friend class PhysicsWorld;
public:
	RigidBody();
	explicit RigidBody(GameObject* pObject, RigidType type);
	explicit RigidBody(GameObject* pObject, RigidType type, Shape* shape);


	~RigidBody();


private:
	GameObject* object;
	Matrix2D* transform;

	D3DXVECTOR2 velocity;
	float angularVelocity;
	D3DXVECTOR2 force;
	float torque;
	float orient; // radians

	RigidType type;
	bool bActive;

	float inertia;
	float InvInertia;
	float mass;
	float InvMass;

	Shape* shape;
	float staticFriction;
	float dynamicFriction;
	float restitution;

public:
	void SetVelocity(const D3DXVECTOR2& v) { velocity = v; }
	const D3DXVECTOR2& GetVelocity() { return velocity; }

	void SetForce(const D3DXVECTOR2& val) { force += val; }
	void SetForce(const D3DXVECTOR2& normal, const float& val) { force += val * normal; }
	void ApplyImpulse(const D3DXVECTOR2& impulse, const D3DXVECTOR2& contactVector)
	{
		velocity += InvMass * impulse;
		angularVelocity += InvInertia * D3DXVec2Dot(&impulse, &contactVector);
	}
	void SetOrient(float radians);


	void SetStatic(void)
	{
		inertia = 0.0f;
		InvInertia = 0.0f;
		mass = 0.0f;
		InvMass = 0.0f;
	}
	void SetMass(float val)
	{
		if (val < Math::Epsilon) return;
		mass = val;
		InvMass = 1 / val;
	}


	void SetActive(bool b) { bActive = b; }
	bool GetActive() { return bActive; }


	Matrix2D* Transform() { return transform; }
	Shape* GetShape() { return shape; }
};

