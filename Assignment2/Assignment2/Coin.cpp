#include "Coin.h"

Coin::Coin(std::string meshFilePath) : TextGameObject(meshFilePath)
{
	LoadDiffuseTexture("Assets/stars.raw");

	mCollisionBox.dimensions.x = 2.0f;
	mCollisionBox.dimensions.y = 2.0f;
	mCollisionBox.dimensions.z = 2.0f;

	GeneratePosition();
}

void Coin::Update()
{
	GameObject::Update();
	mCollisionBox.position.x = mPosition.x - (mCollisionBox.dimensions.x / 2);
	mCollisionBox.position.y = mPosition.y - (mCollisionBox.dimensions.y / 2);
	mCollisionBox.position.z = mPosition.z - (mCollisionBox.dimensions.z / 2);

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
