#pragma once
#include "OBJGameObject.h"
class SpaceShip : public OBJGameObject
{
public:
	SpaceShip(std::string meshFilePath);

	void Update() override;

	void PollInput(unsigned char key, int x, int y) override;
	void KeyUp(unsigned char key, int x, int y);

	Sphere GetSphere() const { return mCollisionSphere; }

	AABB GetBox() const { return mCollisionBox; }

private:
	Sphere mCollisionSphere;
	AABB mCollisionBox;
};

