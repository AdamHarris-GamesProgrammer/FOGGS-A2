#include "SpaceShip.h"
#include <algorithm>

SpaceShip::SpaceShip(std::string meshFilePath) : OBJGameObject(meshFilePath)
{
	LoadDiffuseTexture(std::string("Assets/") + mObjectMesh[1].meshMaterial.map_Kd);

	SetFriction(0.95f);
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	SetScale(Vector3(0.5f, 0.5f, 0.5f));
}

void SpaceShip::Update()
{

	GameObject::UpdatePosition();

	mPosition.x = std::clamp(mPosition.x, -14.5f, 14.5f);
	mPosition.y = std::clamp(mPosition.y, -15.0f, 15.0f);

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
