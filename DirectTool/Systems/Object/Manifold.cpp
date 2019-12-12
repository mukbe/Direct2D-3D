#include "stdafx.h"
#include "Manifold.h"
#include "RigidBody.h"
#include "Collision.h"

void Manifold::Solve()
{
	Dispatch[A->shape->GetType()][B->shape->GetType()](this, A, B);
}

void Manifold::Initialize()
{
	e = Math::Min(A->restitution, B->restitution);

	// Calculate static and dynamic friction
	sf = std::sqrt(A->staticFriction * A->staticFriction);
	df = std::sqrt(A->dynamicFriction * A->dynamicFriction);

	for (int i = 0; i < contact_count; ++i)
	{
		// Calculate radii from COM to contact
		D3DXVECTOR2 ra = contacts[i] - A->transform->GetPos();
		D3DXVECTOR2 rb = contacts[i] - B->transform->GetPos();

		D3DXVECTOR2 rv = B->velocity + Math::Cross(B->angularVelocity, rb) -
			A->velocity - Math::Cross(A->angularVelocity, ra);


		// Determine if we should perform a resting collision or not
		// The idea is if the only thing moving this object is gravity,
		// then the collision should be performed without any restitution
		if (D3DXVec2Length(&rv) < D3DXVec2Length(&(Time::Delta() * Math::gravity)) + Math::Epsilon)
			e = 0.0f;
	}

}

void Manifold::ApplyImpulse()
{
	// Early out and positional correct if both objects have infinite mass
	if (Math::FloatEqual(A->InvMass + B->InvMass, 0))
	{
		InfiniteMassCorrection();
		return;
	}

	for (int i = 0; i < contact_count; ++i)
	{
		// Calculate radii from COM to contact
		D3DXVECTOR2 ra = contacts[i] - A->transform->GetPos();
		D3DXVECTOR2 rb = contacts[i] - B->transform->GetPos();

		// Relative velocity
		D3DXVECTOR2 rv = B->velocity + Math::Cross(B->angularVelocity, rb) -
			A->velocity - Math::Cross(A->angularVelocity, ra);

		// Relative velocity along the normal
		float contactVel = D3DXVec2Dot(&rv, &normal);

		// Do not resolve if velocities are separating
		if (contactVel > 0)
			return;

		float raCrossN = Math::Cross(ra, normal);
		float rbCrossN = Math::Cross(rb, normal);
		float invMassSum = A->InvMass + B->InvMass + raCrossN* raCrossN * A->InvInertia + rbCrossN* rbCrossN * B->InvInertia;

		// Calculate impulse scalar
		float j = -(1.0f + e) * contactVel;
		j /= invMassSum;
		j /= (float)contact_count;

		// Apply impulse
		D3DXVECTOR2 impulse = normal * j;
		A->ApplyImpulse(-impulse, ra);
		B->ApplyImpulse(impulse, rb);

		// Friction impulse
		rv = B->velocity + Math::Cross(B->angularVelocity, rb) -
			A->velocity - Math::Cross(A->angularVelocity, ra);

		D3DXVECTOR2 t = rv - (normal * D3DXVec2Dot(&rv, &normal));
		D3DXVec2Normalize(&t,&t);

		// j tangent magnitude
		float jt = -D3DXVec2Dot(&rv, &t);
		jt /= invMassSum;
		jt /= (float)contact_count;

		// Don't apply tiny friction impulses
		if (Math::FloatEqual(jt, 0.0f))
			return;

		// Coulumb's law
		D3DXVECTOR2 tangentImpulse;
		if (std::abs(jt) < j * sf)
			tangentImpulse = t * jt;
		else
			tangentImpulse = t * -j * df;

		// Apply friction impulse
		A->ApplyImpulse(-tangentImpulse, ra);
		B->ApplyImpulse(tangentImpulse, rb);
	}

}

void Manifold::PositionalCorrection()
{
	const float k_slop = 0.05f; // Penetration allowance
	const float percent = 0.4f; // Penetration percentage to correct
	D3DXVECTOR2 correction = (Math::Max(penetration - k_slop, 0.0f) / (A->InvMass + B->InvMass)) * normal * percent;

	A->transform->SetPos(A->transform->GetPos() - correction * A->InvMass);
	B->transform->SetPos(B->transform->GetPos() + correction * B->InvMass);
}

void Manifold::InfiniteMassCorrection()
{
	A->velocity = D3DXVECTOR2(0, 0);
	B->velocity = D3DXVECTOR2(0, 0);
}
