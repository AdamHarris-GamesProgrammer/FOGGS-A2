#include "SpaceShip.h"

SpaceShip::SpaceShip(std::string meshFilePath) : GameObject(meshFilePath)
{
	LoadDiffuseTexture(std::string("Assets/") + mObjectMesh[1].meshMaterial.map_Kd);

	SetFriction(0.95f);
}

void SpaceShip::Update()
{
	GameObject::UpdatePosition();
}

void SpaceShip::PollInput(unsigned char key, int x, int y)
{

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
