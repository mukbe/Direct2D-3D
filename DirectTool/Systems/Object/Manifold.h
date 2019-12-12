#pragma once

class RigidBody;
struct Manifold
{
public:
	Manifold(RigidBody *a, RigidBody *b)
		: A(a)
		, B(b)
	{
	}
	~Manifold() {}

	void Solve();                 // Generate contact information
	void Initialize();            // Precalculations for impulse solving
	void ApplyImpulse();          // Solve impulse and apply
	void PositionalCorrection();  // Naive correction of positional penetration
	void InfiniteMassCorrection();


	RigidBody *A;
	RigidBody *B;

	float penetration;     // Depth of penetration from collision
	D3DXVECTOR2 normal;          // From A to B
	D3DXVECTOR2 contacts[2];     // Points of contact during collision
	int contact_count; // Number of contacts that occured during collision
	float e;               // Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction

};

