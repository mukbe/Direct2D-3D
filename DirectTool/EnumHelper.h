#pragma once
enum Pivot
{
	LEFT_TOP, CENTER, BOTTOM, TOP
};
enum ObjectType : short
{
	Background = 0, Charactor, Monster, Object, Effect, UI, ObjectType_End
};
enum RigidType : short
{
	Dynamic = 0, Static, RigidType_End
};