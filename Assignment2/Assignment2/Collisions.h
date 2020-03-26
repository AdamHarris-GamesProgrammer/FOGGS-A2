#pragma once

#include "GraphicsStructures.h"
#include "MathStructures.h"

class Collisions
{
public:
	bool CollisionCheck(Sphere s1, Sphere s2);
	bool CollisionCheck(AABB a, AABB b);
};

