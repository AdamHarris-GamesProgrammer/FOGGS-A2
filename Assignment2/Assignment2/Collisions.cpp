#include "Collisions.h"

bool Collisions::CollisionCheck(AABB a, AABB b)
{
	float minAX = a.position.x;
	float maxAX = a.position.x + a.dimensions.x;
	float minAY = a.position.y;
	float maxAY = a.position.y + a.dimensions.y;
	float minAZ = a.position.z;
	float maxAZ = a.position.z + a.dimensions.z;

	float minBX = b.position.x;
	float maxBX = b.position.x + b.dimensions.x;
	float minBY = b.position.y;
	float maxBY = b.position.y + b.dimensions.y;
	float minBZ = b.position.z;
	float maxBZ = b.position.z + b.dimensions.z;

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
