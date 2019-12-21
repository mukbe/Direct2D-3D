#include "stdafx.h"
#include "RigidBody.h"
#include "Shape.h"


RigidBody::RigidBody()
{
}

RigidBody::RigidBody(GameObject * pObject, RigidType type)
	:object(pObject), velocity(0,0), force(0,0), type(type), bActive(true)
{
	transform = object->Transform();

}

RigidBody::RigidBody(GameObject * pObject, RigidType type, Shape * shape)
	:object(pObject), velocity(0, 0), force(0, 0), type(type), bActive(true), shape(shape->Clone())
{
	transform = object->Transform();
	this->shape->body = this;
	angularVelocity = 0;
	torque = 0;
	SetOrient(-Math::PI * 0.5f * 0.f);
	staticFriction = 0.5f;
	dynamicFriction = 0.3f;
	restitution = 0.2f;
	this->shape->Initialize();
}


RigidBody::~RigidBody()
{
}

void RigidBody::SetOrient(float radians)
{
	transform->SetRadian(radians);
	shape->SetOrient(radians);
}

void RigidBody::SetMass(float val, float density)
{
	if (val < Math::Epsilon) return;
	mass = val;
	shape->ComputeMass(density);


}

