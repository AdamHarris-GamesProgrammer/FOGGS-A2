#pragma once

#include "GLIncludes.h"

#include "MathStructures.h"

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera {
public:
	Vector3 mPosition;
	Vector3 mFront;
	Vector3 mRight;
	Vector3 mWorldUp;
	Vector3 mUp;

	float mYaw;
	float mPitch;

	float mMovementSpeed;
	float mMouseSensitivity;

	//Camera Constructor using Vectors
	Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
		mFront(Vector3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY)
	{
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		UpdateCameraVectors();
	}

	//Camera Constructor using Scalars
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		mFront(0.0f, 0.0f, -1.0f), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY)
	{
		mPosition = Vector3(posX, posY, posZ);
		mWorldUp = Vector3(upX, upY, upZ);
		mYaw = yaw;
		mPitch = pitch;
		UpdateCameraVectors();
	}

	void ProcessInput(float xOffset, float yOffset, bool contrainPitch = true) {
		xOffset *= mMouseSensitivity;
		yOffset *= mMouseSensitivity;

		mYaw += xOffset;
		mPitch += yOffset;

		if (contrainPitch) {
			if (mPitch > 89.0f) {
				mPitch = 89.0f;
			}
			if (mPitch < -89.0f) {
				mPitch = -89.0f;
			}
		}

		UpdateCameraVectors();
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = mMovementSpeed * deltaTime;
		if (direction == FORWARD)
			mPosition = mPosition + (mFront * velocity);
		if (direction == BACKWARD)
			mPosition = mPosition - (mFront * velocity);
		if (direction == LEFT)
			mPosition = mPosition - (mRight * velocity);
		if (direction == RIGHT)
			mPosition = mPosition + (mRight * velocity);
	}

private:
	void UpdateCameraVectors() {
		Vector3 front;
		front.x = cos(Math::DegreeToRadians(mYaw)) * cos(Math::DegreeToRadians(mPitch));
		front.y = sin(Math::DegreeToRadians(mPitch));
		front.z = sin(Math::DegreeToRadians(mYaw)) * cos(Math::DegreeToRadians(mPitch));
		mFront = Math::Normalise(front);

		mRight = Math::Normalise(Math::CrossProduct(mFront, mWorldUp));
		mUp = Math::Normalise(Math::CrossProduct(mRight, mFront));
	}
};
