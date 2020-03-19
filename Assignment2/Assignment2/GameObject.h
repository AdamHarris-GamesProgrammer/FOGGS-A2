#pragma once

#include "MathStructures.h"

class GameObject
{
public:
	virtual void Render();
	virtual void Update();

	virtual void UpdatePosition();
private:

protected:
	Vector3 mPosition;
	Vector3 mVelocity;

	Rotation mRotation;

	float mObjectFriction = 0.05f;
};

