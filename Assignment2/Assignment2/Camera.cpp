#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 eye, Vector3 center, Vector3 up)
{
	mOriginalPosition = position;
	mOriginalCenter = center;
	mOriginalEye = eye;
	mOriginalUp = up;

	mPosition = mOriginalPosition;
	mCenter = mOriginalCenter;
	mEye = mOriginalEye;
	mUp = mOriginalUp;
}

Camera::~Camera()
{

}

void Camera::ResetCamera()
{
	//Resets all values to their originals
	mPosition = mOriginalPosition;
	mCenter = mOriginalCenter;
	mEye = mOriginalEye;
	mUp = mOriginalUp;
}

void Camera::Update()
{
	gluLookAt(mEye.x, mEye.y, mEye.z, mCenter.x, mCenter.y, mCenter.z, mUp.x, mUp.y, mUp.z);
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
}

void Camera::ProcessInput(float xOffset, float yOffset)
{
	const float sensitivity = 0.5f;
	yOffset *= sensitivity;
	xOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	pitch = std::clamp(pitch, -89.0f, 89.0f);

	Vector3 direction;
	direction.x = cos(Math::DegreeToRadians(yaw)) * cos(Math::DegreeToRadians(pitch));
	direction.y = sin(Math::DegreeToRadians(pitch));
	direction.z = sin(Math::DegreeToRadians(yaw)) * cos(Math::DegreeToRadians(pitch));


	mCenter = Math::Normalise(direction);
}
