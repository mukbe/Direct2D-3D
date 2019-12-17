#include "stdafx.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Manifold.h"
#include "Shape.h"

//TODO ���̾ �ΰ� �浹üũ�� �ϰԵǸ� ȿ���� �������� ���� 
//		�������� ���� ��� �Ծؿɰ��� ������ �� �������� ������ ���� �� �����Ű���

//����ȭ�� ������� �ؾ��ϳ� 
// ���޽� ����� ���δ� => �ε巴�� ���� �� �ִ�
//
PhysicsWorld::PhysicsWorld()
{
	bActive = true;
	m_iterations = 2;
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
			//Draw Point
			D3DXVECTOR2 c = m.contacts[j];
			CAMERA->GetView().Bind();
			FloatRect rc(c, 10.f, Pivot::CENTER);
			p2DRenderer->FillEllipse(rc, DefaultBrush::gray);
		}
	}
	for (int i = 0; i < contacts.size(); ++i)
	{
		Manifold& m = contacts[i];
		D3DXVECTOR2 n = m.normal;
		for (int j = 0; j < m.contact_count; ++j)
		{
			//Draw Line
			D3DXVECTOR2 c = m.contacts[j];
			D3DXVECTOR2 c2 = c;
			CAMERA->GetView().Bind();
			n *= 0.75f;
			c2 += n;
			p2DRenderer->DrawLine(c,c2,DefaultBrush::yello);
		}
	}

}

RigidBody * PhysicsWorld::Add(GameObject * obj,RigidType type, Shape* shape)
{
	//rigidtype�ϰ� shapetype ���� �ϳ��� �����ؾߵɵ��ϴ�
	RigidBody* body = new RigidBody(obj, type, shape);


	bodies.push_back(body);
	return body;
}

void PhysicsWorld::IntegrateForces(RigidBody * b)
{
	if (b->InvMass == 0.0f)
		return;

	b->velocity += (b->force * b->InvMass + Math::gravity) * (Time::Tick() );
	b->angularVelocity += b->torque * b->InvInertia * (Time::Tick());
}

void PhysicsWorld::IntegrateVelocity(RigidBody * b)
{
	if (b->InvMass == 0.0f)
		return;

	Matrix2D* transform = b->transform;

	transform->SetPos(transform->GetPos() + b->velocity * Time::Tick());
	transform->rotation += b->angularVelocity * Time::Tick();
	b->SetOrient(transform->rotation);
	IntegrateForces(b);
}
