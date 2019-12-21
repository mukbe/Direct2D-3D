#include "stdafx.h"
#include "Shape.h"
#include "./Utilities/Matrix2x2.h"
#include "./Systems/Object/RigidBody.h"


float Shape::inertiaFactor = 0.1f;

void Circle::ComputeMass(float density)
{
	//body->mass = Math::PI * radius * radius * density;
	body->InvMass = (body->mass) ? 1.0f / body->mass : 0.0f;
	body->inertia = body->mass * radius * radius * inertiaFactor;
	body->InvInertia = (body->inertia) ? 1.0f / body->inertia : 0.0f;

}

void Circle::Draw(void)
{
	Matrix2D world = *body->transform;

	world = world * CAMERA->GetView();
	world.Bind();

	D3DXVECTOR2 start = { 0,0 };
	D3DXVECTOR2 end = { radius, 0 };
	end += start;
	p2DRenderer->DrawLine(start, end, DefaultBrush::red);
	p2DRenderer->DrawEllipse(body->transform->GetRect(), DefaultBrush::blue);
	body->transform->Render();
}

void PolygonShape::ComputeMass(float density)
{
	// Calculate centroid and moment of interia
	D3DXVECTOR2 c(0.0f, 0.0f); // centroid
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (int i1 = 0; i1 < m_vertexCount; ++i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		D3DXVECTOR2 p1(m_vertices[i1]);
		int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		D3DXVECTOR2 p2(m_vertices[i2]);

		float D = Math::Cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * k_inv3 * (p1 + p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	for (int i = 0; i < m_vertexCount; ++i)
		m_vertices[i] -= c;

	//body->mass = density * area;
	body->InvMass = (body->mass) ? 1.0f / body->mass : 0.0f;
	body->inertia = body->mass* I * density * inertiaFactor  * 0.001f;
	body->InvInertia = body->inertia ? 1.0f / body->inertia : 0.0f;


}

void PolygonShape::Draw(void) 
{
	Matrix2D world = *body->transform;

	world = world * CAMERA->GetView();
	world.Bind();

	p2DRenderer->DrawRectangle(body->transform->GetRect(), DefaultBrush::blue);
	body->transform->Render();

}
