#pragma once

#include "GraphicsStructures.h"
#include "MathStructures.h"
#include <iostream>
#include <algorithm>

class Camera
{
public:
	Camera(Vector3 position, Vector3 eye, Vector3 center, Vector3 up);
	~Camera();

	void ResetCamera();
	void Update();

	void ProcessInput(float xOffset, float yOffset);
private:
	Vector3 mPosition;
	Vector3 mEye;
	Vector3 mCenter;
	Vector3 mUp;

	Vector3 mOriginalPosition;
	Vector3 mOriginalEye;
	Vector3 mOriginalCenter;
	Vector3 mOriginalUp;

	float yaw;
	float pitch;
};

