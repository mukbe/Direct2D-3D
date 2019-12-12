#include "stdafx.h"
#include "Collision.h"
#include "RigidBody.h"
#include "./Utilities/Matrix2D.h"
#include "Manifold.h"

CollisionCallback Dispatch[Shape::Shape_End][Shape::Shape_End] =
{
  {
	CircletoCircle, CircletoPolygon
  },
  {
	PolygontoCircle, PolygontoPolygon
  },
};

void CircletoCircle(Manifold *m, RigidBody *a, RigidBody *b)
{
	Circle *A = reinterpret_cast<Circle *>(a->GetShape());
	Circle *B = reinterpret_cast<Circle *>(b->GetShape());

	// Calculate translational vector, which is normal
	D3DXVECTOR2 normal = b->Transform()->GetPos() - a->Transform()->GetPos();

	float dist_sqr = normal.x * normal.x + normal.y * normal.y;
	float radius = A->radius + B->radius;

	// Not in contact
	if (dist_sqr >= radius * radius)
	{
		m->contact_count = 0;
		return;
	}

	float distance = std::sqrt(dist_sqr);

	m->contact_count = 1;

	if (distance == 0.0f)
	{
		m->penetration = A->radius;
		m->normal = D3DXVECTOR2(1, 0);
		m->contacts[0] = a->Transform()->GetPos();;
	}
	else
	{
		m->penetration = radius - distance;
		m->normal = normal / distance; // Faster than using Normalized since we already performed sqrt
		m->contacts[0] = m->normal * A->radius + a->Transform()->GetPos();;
	}
}

void CircletoPolygon(Manifold *m, RigidBody *a, RigidBody *b)
{
	Circle *A = reinterpret_cast<Circle *>      (a->GetShape());
	PolygonShape *B = reinterpret_cast<PolygonShape *>(b->GetShape());

	m->contact_count = 0;

	// Transform circle center to Polygon model space
	D3DXVECTOR2 center = a->Transform()->GetPos();
	center = B->mat.Transpose() * (center - b->Transform()->GetPos());

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separation = -FLT_MAX;
	int faceNormal = 0;
	for (int i = 0; i < B->m_vertexCount; ++i)
	{
		float s = D3DXVec2Dot(&B->m_normals[i], &(center - B->m_vertices[i])); 

		if (s > A->radius)
			return;

		if (s > separation)
		{
			separation = s;
			faceNormal = i;
		}
	}

	// Grab face's vertices
	D3DXVECTOR2 v1 = B->m_vertices[faceNormal];
	int i2 = faceNormal + 1 < B->m_vertexCount ? faceNormal + 1 : 0;
	D3DXVECTOR2 v2 = B->m_vertices[i2];

	// Check to see if center is within polygon
	if (separation < Math::Epsilon)
	{
		m->contact_count = 1;
		m->normal = -(B->mat * B->m_normals[faceNormal]);
		m->contacts[0] = m->normal * A->radius + a->Transform()->GetPos();
		m->penetration = A->radius;
		return;
	}

	// Determine which voronoi region of the edge center of circle lies within
	float dot1 = D3DXVec2Dot(&(center - v1), &(v2 - v1));
	float dot2 = D3DXVec2Dot(&(center - v2), &(v1 - v2));
	m->penetration = A->radius - separation;

	// Closest to v1
	if (dot1 <= 0.0f)
	{
		D3DXVECTOR2 d = center - v1;
		if (D3DXVec2Dot(&d, &d) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		D3DXVECTOR2 n = v1 - center;
		n = B->mat * n;
		D3DXVec2Normalize(&n, &n);
		m->normal = n;
		v1 = B->mat * v1 + b->Transform()->GetPos();
		m->contacts[0] = v1;
	}

	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		D3DXVECTOR2 d = center - v2;
		if (D3DXVec2Dot(&d, &d) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		D3DXVECTOR2 n = v2 - center;
		v2 = B->mat * v2 + b->Transform()->GetPos();
		m->contacts[0] = v2;
		n = B->mat * n;
		D3DXVec2Normalize(&n, &n);
		m->normal = n;
	}

	// Closest to face
	else
	{
		D3DXVECTOR2 n = B->m_normals[faceNormal];
		if (D3DXVec2Dot(&(center - v1), &n) > A->radius)
			return;

		n = B->mat * n;
		m->normal = -n;
		m->contacts[0] = m->normal * A->radius + a->Transform()->GetPos();
		m->contact_count = 1;
	}
}

void PolygontoCircle(Manifold *m, RigidBody *a, RigidBody *b)
{
	CircletoPolygon(m, b, a);
	m->normal = -m->normal;
}

float FindAxisLeastPenetration(int *faceIndex, PolygonShape *A, PolygonShape *B)
{
	float bestDistance = -FLT_MAX;
	int bestIndex;

	for (int i = 0; i < A->m_vertexCount; ++i)
	{
		// Retrieve a face normal from A
		D3DXVECTOR2 n = A->m_normals[i];
		D3DXVECTOR2 nw = A->mat * n;

		// Transform face normal into B's model space
		Matrix2x2 buT = B->mat.Transpose();
		n = buT * nw;

		// Retrieve support point from B along -n
		D3DXVECTOR2 s = B->GetSupport(-n);

		// Retrieve vertex on face from A, transform into
		// B's model space
		D3DXVECTOR2 v = A->m_vertices[i];
		v = A->mat * v + A->body->Transform()->GetPos();
		v -= B->body->Transform()->GetPos();
		v = buT * v;

		// Compute penetration distance (in B's model space)
		float d = D3DXVec2Dot(&n, &(s - v));

		// Store greatest distance
		if (d > bestDistance)
		{
			bestDistance = d;
			bestIndex = i;
		}
	}

	*faceIndex = bestIndex;
	return bestDistance;
}

void FindIncidentFace(D3DXVECTOR2 *v, PolygonShape *RefPoly, PolygonShape *IncPoly, int referenceIndex)
{
	D3DXVECTOR2 referenceNormal = RefPoly->m_normals[referenceIndex];

	// Calculate normal in incident's frame of reference
	referenceNormal = RefPoly->mat * referenceNormal; // To world space
	referenceNormal = IncPoly->mat.Transpose() * referenceNormal; // To incident's model space

	// Find most anti-normal face on incident polygon
	int incidentFace = 0;
	float minDot = FLT_MAX;
	for (int i = 0; i < IncPoly->m_vertexCount; ++i)
	{
		float dot = D3DXVec2Dot(&referenceNormal, &IncPoly->m_normals[i]);
		if (dot < minDot)
		{
			minDot = dot;
			incidentFace = i;
		}
	}

	// Assign face vertices for incidentFace
	v[0] = IncPoly->mat * IncPoly->m_vertices[incidentFace] + IncPoly->body->Transform()->GetPos();
	incidentFace = incidentFace + 1 >= (int)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
	v[1] = IncPoly->mat * IncPoly->m_vertices[incidentFace] + IncPoly->body->Transform()->GetPos();
}

int Clip(D3DXVECTOR2 n, float c, D3DXVECTOR2 *face)
{
	int sp = 0;
	D3DXVECTOR2 out[2] = {
	  face[0],
	  face[1]
	};

	// Retrieve distances from each endpoint to the line
	// d = ax + by - c
	float d1 = D3DXVec2Dot(&n, &face[0]) - c;
	float d2 = D3DXVec2Dot(&n, &face[1]) - c;

	// If negative (behind plane) clip
	if (d1 <= 0.0f) out[sp++] = face[0];
	if (d2 <= 0.0f) out[sp++] = face[1];

	// If the points are on different sides of the plane
	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
	{
		// Push interesection point
		float alpha = d1 / (d1 - d2);
		out[sp] = face[0] + alpha * (face[1] - face[0]);
		++sp;
	}

	// Assign our new converted values
	face[0] = out[0];
	face[1] = out[1];

	assert(sp != 3);

	return sp;
}

void PolygontoPolygon(Manifold *m, RigidBody *a, RigidBody *b)
{
	PolygonShape *A = reinterpret_cast<PolygonShape *>(a->GetShape());
	PolygonShape *B = reinterpret_cast<PolygonShape *>(b->GetShape());
	m->contact_count = 0;

	// Check for a separating axis with A's face planes
	int faceA;
	float penetrationA = FindAxisLeastPenetration(&faceA, A, B);
	if (penetrationA >= 0.0f)
		return;

	// Check for a separating axis with B's face planes
	int faceB;
	float penetrationB = FindAxisLeastPenetration(&faceB, B, A);
	if (penetrationB >= 0.0f)
		return;

	int referenceIndex;
	bool flip; // Always point from a to b

	PolygonShape *RefPoly; // Reference
	PolygonShape *IncPoly; // Incident

	// Determine which shape contains reference face
	if (Math::BiasGreaterThan(penetrationA, penetrationB))
	{
		RefPoly = A;
		IncPoly = B;
		referenceIndex = faceA;
		flip = false;
	}

	else
	{
		RefPoly = B;
		IncPoly = A;
		referenceIndex = faceB;
		flip = true;
	}

	// World space incident face
	D3DXVECTOR2 incidentFace[2];
	FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);

	//        y
	//        ^  ->n       ^
	//      +---c ------posPlane--
	//  x < | i |\
    //      +---+ c-----negPlane--
	//             \       v
	//              r
	//
	//  r : reference face
	//  i : incident poly
	//  c : clipped point
	//  n : incident normal

	// Setup reference face vertices
	D3DXVECTOR2 v1 = RefPoly->m_vertices[referenceIndex];
	referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
	D3DXVECTOR2 v2 = RefPoly->m_vertices[referenceIndex];

	// Transform vertices to world space
	v1 = RefPoly->mat * v1 + RefPoly->body->Transform()->GetPos();
	v2 = RefPoly->mat * v2 + RefPoly->body->Transform()->GetPos();

	// Calculate reference face side normal in world space
	D3DXVECTOR2 sidePlaneNormal = (v2 - v1);
	D3DXVec2Normalize(&sidePlaneNormal,&sidePlaneNormal);

	// Orthogonalize
	D3DXVECTOR2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

	// ax + by = c
	// c is distance from origin
	float refC = D3DXVec2Dot(&refFaceNormal, &v1);
	float negSide = -D3DXVec2Dot(&sidePlaneNormal, &v1);
	float posSide = D3DXVec2Dot(&sidePlaneNormal, &v2);

	// Clip incident face to reference face side planes
	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	  // Flip
	m->normal = flip ? -refFaceNormal : refFaceNormal;

	// Keep points behind reference face
	int cp = 0; // clipped points behind reference face
	float separation = D3DXVec2Dot(&refFaceNormal, &incidentFace[0]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[0];
		m->penetration = -separation;
		++cp;
	}
	else
		m->penetration = 0;

	separation = D3DXVec2Dot(&refFaceNormal, &incidentFace[1]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[1];

		m->penetration += -separation;
		++cp;

		// Average penetration
		m->penetration /= (float)cp;
	}

	m->contact_count = cp;
}
