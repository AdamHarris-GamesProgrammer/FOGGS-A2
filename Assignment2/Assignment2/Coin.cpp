#include "Coin.h"

Coin::Coin(std::string meshFilePath) : TextGameObject(meshFilePath)
{
	LoadDiffuseTexture("Assets/stars.raw");

	mCollisionSphere.position = mPosition;
	mCollisionSphere.radius = 1.0f;

	mCollisionBox.w = 2.0f;
	mCollisionBox.h = 2.0f;
	mCollisionBox.d = 2.0f;

	GeneratePosition();
}

void Coin::Update()
{
	GameObject::Update();
	mCollisionSphere.position = mPosition;

	mCollisionBox.x = mPosition.x - (mCollisionBox.w / 2);
	mCollisionBox.y = mPosition.y - (mCollisionBox.h / 2);
	mCollisionBox.z = mPosition.z - (mCollisionBox.d / 2);

	mRotation.x += 0.1f;
	mRotation.y += 0.2f;
	mRotation.z += 0.3f;

	//printf("Coin\nX: %f\tX+W: %f\nY: %f\tY+H: %f\nZ: %f\t Z+D: %f\n",
	//	mCollisionBox.x, mCollisionBox.x + mCollisionBox.w,
	//	mCollisionBox.y, mCollisionBox.y + mCollisionBox.h,
	//	mCollisionBox.z, mCollisionBox.z + mCollisionBox.d);

}

void Coin::GeneratePosition()
{
	//Generates position within screen bounds
	mPosition.x = (rand() % 31) - 15.5f;
	mPosition.y = (rand() % 31) - 15.5f;
}
