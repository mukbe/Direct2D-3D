#pragma once

#define MaxPolyVertexCount 64

class RigidBody;
class Matrix2x2;
struct Shape
{
	enum ShapeType
	{
		Shape_Circle,
		Shape_Poly,
		Shape_End
	};
	Shape() {}
	virtual Shape* Clone() const = 0;
	virtual void Initialize() = 0;
	virtual void ComputeMass(float density) = 0;
	virtual void SetOrient(float radians) = 0;
	virtual void Draw() {}
	virtual ShapeType GetType() = 0;

	RigidBody* body;
	float radius;
	
	Matrix2x2 mat;
};

struct Circle : public Shape
{
	Circle(float r)
	{
		radius = r;
	}

	Shape *Clone(void) const
	{
		return new Circle(radius);
	}

	void Initialize()
	{
		ComputeMass(1.0f);
	}

	void ComputeMass(float density);

	void SetOrient(float radians) {}

	void Draw(void);
	

	ShapeType GetType(void)
	{
		return ShapeType::Shape_Circle;
	}
};

struct PolygonShape : public Shape
{
	void Initialize(void)
	{
		ComputeMass(1.0f);
	}

	Shape *Clone(void) const
	{
		PolygonShape *poly = new PolygonShape();
		poly->mat = mat;
		for (int i = 0; i < m_vertexCount; ++i)
		{
			poly->m_vertices[i] = m_vertices[i];
			poly->m_normals[i] = m_normals[i];
		}
		poly->m_vertexCount = m_vertexCount;
		return poly;
	}

	void ComputeMass(float density);


	void SetOrient(float radians)
	{
		mat.Set(radians);
	}

	void Draw(void) ;

	ShapeType GetType()
	{
		return ShapeType::Shape_Poly;
	}

	// Half width and half height
	void SetBox(float hw, float hh)
	{
		m_vertexCount = 4;
		m_vertices[0] = D3DXVECTOR2(-hw, -hh);
		m_vertices[1] = D3DXVECTOR2(hw, -hh);
		m_vertices[2] = D3DXVECTOR2(hw, hh);
		m_vertices[3] = D3DXVECTOR2(-hw, hh);
		m_normals[0]  = D3DXVECTOR2(0.0f, -1.0f);
		m_normals[1]  = D3DXVECTOR2(1.0f, 0.0f);
		m_normals[2]  = D3DXVECTOR2(0.0f, 1.0f);
		m_normals[3]  = D3DXVECTOR2(-1.0f, 0.0f);
	}

	void Set(D3DXVECTOR2 *vertices, int count)
	{
		// No hulls with less than 3 vertices (ensure actual polygon)
		assert(count > 2 && count <= MaxPolyVertexCount);
		count = Math::Min((int)count, MaxPolyVertexCount);

		// Find the right most point on the hull
		int rightMost = 0;
		float highestXCoord = vertices[0].x;
		for (int i = 1; i < count; ++i)
		{
			float x = vertices[i].x;
			if (x > highestXCoord)
			{
				highestXCoord = x;
				rightMost = i;
			}

			// If matching x then take farthest negative y
			else if (x == highestXCoord)
				if (vertices[i].y < vertices[rightMost].y)
					rightMost = i;
		}

		int hull[MaxPolyVertexCount];
		int outCount = 0;
		int indexHull = rightMost;

		for (;;)
		{
			hull[outCount] = indexHull;

			// Search for next index that wraps around the hull
			// by computing cross products to find the most counter-clockwise
			// vertex in the set, given the previos hull index
			int nextHullIndex = 0;
			for (int i = 1; i < (int)count; ++i)
			{
				// Skip if same coordinate as we need three unique
				// points in the set to perform a cross product
				if (nextHullIndex == indexHull)
				{
					nextHullIndex = i;
					continue;
				}

				// Cross every set of three unique vertices
				// Record each counter clockwise third vertex and add
				// to the output hull
				// See : http://www.oocities.org/pcgpe/math2d.html
				D3DXVECTOR2	 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
				D3DXVECTOR2	 e2 = vertices[i] - vertices[hull[outCount]];
				float c = Math::Cross(e1, e2);
				if (c < 0.0f)
					nextHullIndex = i;

				// Cross product is zero then e vectors are on same line
				// therefor want to record vertex farthest along that line
				if (c == 0.0f && D3DXVec2Length(&e1) > D3DXVec2Length(&e1))
					nextHullIndex = i;
			}

			++outCount;
			indexHull = nextHullIndex;

			// Conclude algorithm upon wrap-around
			if (nextHullIndex == rightMost)
			{
				m_vertexCount = outCount;
				break;
			}
		}

		// Copy vertices into shape's vertices
		for (int i = 0; i < m_vertexCount; ++i)
			m_vertices[i] = vertices[hull[i]];

		// Compute face normals
		for (int i1 = 0; i1 < m_vertexCount; ++i1)
		{
			int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
			D3DXVECTOR2 face = m_vertices[i2] - m_vertices[i1];

			// Ensure no zero-length edges, because that's bad
			assert(D3DXVec2Length(&face) > Math::Epsilon * Math::Epsilon);

			// Calculate normal with 2D cross product between vector and scalar
			m_normals[i1] = D3DXVECTOR2(face.y, -face.x);
			D3DXVec2Normalize(&m_normals[i1], &m_normals[i1]);
		}
	}

	// The extreme point along a direction within a polygon
	D3DXVECTOR2 GetSupport(const D3DXVECTOR2& dir)
	{
		float bestProjection = -FLT_MAX;
		D3DXVECTOR2 bestVertex;

		for (int i = 0; i < m_vertexCount; ++i)
		{
			D3DXVECTOR2 v = m_vertices[i];
			float projection = D3DXVec2Dot(&v, &dir);

			if (projection > bestProjection)
			{
				bestVertex = v;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}

	float m_vertexCount;
	D3DXVECTOR2 m_vertices[MaxPolyVertexCount];
	D3DXVECTOR2 m_normals[MaxPolyVertexCount];
};

