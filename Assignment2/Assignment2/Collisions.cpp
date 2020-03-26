#include "Collisions.h"

bool Collisions::CollisionCheck(Sphere s1, Sphere s2)
{
	float distance = ((s1.position.x - s2.position.x) * (s1.position.x - s2.position.x)) +
		((s1.position.y - s2.position.y) * (s1.position.y - s2.position.y)) +
		((s1.position.z - s2.position.z) * (s1.position.z - s2.position.z));

	float radiusDistance;

	radiusDistance = pow(s1.radius + s2.radius, 2);

	if (distance <= radiusDistance) {
		return true;
	}
	else
	{
		return false;
	}
}

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
