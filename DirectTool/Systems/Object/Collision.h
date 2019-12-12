#pragma once
#include "Shape.h"

class Manifold;
class RigidBody;
class Matrix2D;
typedef void(*CollisionCallback)(Manifold *m, RigidBody *a, RigidBody *b);

extern CollisionCallback Dispatch[Shape::Shape_End][Shape::Shape_End];

void CircletoCircle(Manifold *m, RigidBody *a, RigidBody *b);
void CircletoPolygon(Manifold *m, RigidBody *a, RigidBody *b);
void PolygontoCircle(Manifold *m, RigidBody *a, RigidBody *b);
void PolygontoPolygon(Manifold *m, RigidBody *a, RigidBody *b);
