#pragma once
#include "TextGameObject.h"
class Coin : public TextGameObject
{
public:
	Coin(std::string meshFilePath);

	void Update() override;

	void GeneratePosition();

	Sphere GetSphere() const { return mCollisionSphere; }
	AABB GetBox() const { return mCollisionBox; }

private:

};

