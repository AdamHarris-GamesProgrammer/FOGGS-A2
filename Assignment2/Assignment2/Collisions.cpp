#include "Collisions.h"

bool Collisions::CollisionCheck(AABB a, AABB b)
{
	float minAX = a.x;
	float maxAX = a.x + a.w;
	float minAY = a.y;
	float maxAY = a.y + a.h;
	float minAZ = a.z;
	float maxAZ = a.z + a.d;

	float minBX = b.x;
	float maxBX = b.x + b.w;
	float minBY = b.y;
	float maxBY = b.y + b.h;
	float minBZ = b.z;
	float maxBZ = b.z + b.d;

	if ((minAX <= maxBX && maxAX >= minBX) &&
		(minAY <= maxBY && maxAY >= minBY) &&
		(minAZ <= maxBZ && maxAZ >= minBZ)) {
		return true;
	}
	else
	{
		return false;
	}
}
