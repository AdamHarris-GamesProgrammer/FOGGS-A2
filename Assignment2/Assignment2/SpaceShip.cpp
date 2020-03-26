#include "SpaceShip.h"
#include <algorithm>

SpaceShip::SpaceShip(std::string meshFilePath) : OBJGameObject(meshFilePath)
{
	LoadDiffuseTexture(std::string("Assets/") + mObjectMesh[1].meshMaterial.map_Kd);

	SetFriction(0.95f);
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));

	mCollisionSphere.radius = 4.0f;

	mCollisionBox.w = 8.0f;
	mCollisionBox.h = 2.0f;
	mCollisionBox.d = 3.0f;
}

void SpaceShip::Update()
{

	GameObject::UpdatePosition();

	mPosition.x = std::clamp(mPosition.x, -14.5f, 14.5f);
	mPosition.y = std::clamp(mPosition.y, -15.0f, 15.0f);

	mCollisionSphere.position = mPosition;

	mCollisionBox.x = mPosition.x - (mCollisionBox.w / 2);
	mCollisionBox.y = mPosition.y - (mCollisionBox.h / 2);
	mCollisionBox.z = mPosition.z - (mCollisionBox.d / 2);

	//printf("Spaceship\nX: %f\tX+W: %f\nY: %f\tY+H: %f\nZ: %f\t Z+D: %f\n", 
	//	mCollisionBox.x, mCollisionBox.x + mCollisionBox.w,
	//	mCollisionBox.y, mCollisionBox.y + mCollisionBox.h, 
	//	mCollisionBox.z, mCollisionBox.z + mCollisionBox.d);
}

void SpaceShip::PollInput(unsigned char key, int x, int y)
{
	if (key == 'a') {
		mVelocity.x = -1.0f;
	}
	else if (key == 'd') {
		mVelocity.x = 1.0f;
	}

	if (key == 'w') {
		mVelocity.y = 1.0f;
	}
	else if (key == 's') {
		mVelocity.y = -1.0f;
	}
}

void SpaceShip::KeyUp(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'd') {
		mVelocity.x = 0.0f;
	}
	
	if (key == 'w' || key == 's') {
		mVelocity.y = 0.0f;
	}
}
