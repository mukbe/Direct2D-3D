#include "stdafx.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Manifold.h"
#include "Shape.h"

PhysicsWorld::PhysicsWorld()
{
	bActive = false;
	m_iterations = 10;
}


PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::Update()
{
	if (bActive == false) return;
	// Generate new collision info
	contacts.clear();
	for (int i = 0; i < bodies.size(); ++i)
	{
		RigidBody *A = bodies[i];

		for (int j = i + 1; j < bodies.size(); ++j)
		{
			RigidBody *B = bodies[j];
			if (A->InvMass == 0 && B->InvMass == 0)
				continue;
			Manifold m(A, B);
			m.Solve();
			if (m.contact_count)
				contacts.emplace_back(m);
		}
	}

	// Integrate forces
	for (int i = 0; i < bodies.size(); ++i)
		IntegrateForces(bodies[i]);

	// Initialize collision
	for (int i = 0; i < contacts.size(); ++i)
		contacts[i].Initialize();

	// Solve collisions
	for (int j = 0; j < m_iterations; ++j)
		for (int i = 0; i < contacts.size(); ++i)
			contacts[i].ApplyImpulse();

	// Integrate velocities
	for (int i = 0; i < bodies.size(); ++i)
		IntegrateVelocity(bodies[i]);

	// Correct positions
	for (int i = 0; i < contacts.size(); ++i)
		contacts[i].PositionalCorrection();

	// Clear all forces
	for (int i = 0; i < bodies.size(); ++i)
	{
		RigidBody *b = bodies[i];
		b->force = D3DXVECTOR2(0, 0);
		b->torque = 0;
	}

}

void PhysicsWorld::Render()
{
	for (int i = 0; i < bodies.size(); ++i)
	{
		RigidBody *b = bodies[i];
		b->shape->Draw();
	}

	for (int i = 0; i < contacts.size(); ++i)
	{
		Manifold& m = contacts[i];
		for (int j = 0; j < m.contact_count; ++j)
		{
			D3DXVECTOR2 c = m.contacts[j];
			//Draw Point
		}
	}
	for (int i = 0; i < contacts.size(); ++i)
	{
		Manifold& m = contacts[i];
		D3DXVECTOR2 n = m.normal;
		for (int j = 0; j < m.contact_count; ++j)
		{
			D3DXVECTOR2 c = m.contacts[j];
			//Draw Line
			//glVertex2f(c.x, c.y);
			//n *= 0.75f;
			//c += n;
			//glVertex2f(c.x, c.y);
		}
	}

}

RigidBody * PhysicsWorld::Add(GameObject * obj,RigidType type, Shape* shape)
{
	//rigidtype하고 shapetype 둘중 하나를 결정해야될듯하다
	RigidBody* body = new RigidBody(obj, type, shape);


	bodies.push_back(body);
	return body;
}

void PhysicsWorld::IntegrateForces(RigidBody * b)
{
	if (b->InvMass == 0.0f)
		return;

	b->velocity += (b->force * b->InvMass + Math::gravity) * (Time::Delta() / 2.0f);
	b->angularVelocity += b->torque * b->InvInertia * (Time::Delta() / 2.0f);
}

void PhysicsWorld::IntegrateVelocity(RigidBody * b)
{
	if (b->InvMass == 0.0f)
		return;

	b->transform->SetPos(b->transform->GetPos() + b->velocity * Time::Delta());
	b->orient += b->angularVelocity * Time::Delta();
	b->SetOrient(b->orient);
	IntegrateForces(b);
}
