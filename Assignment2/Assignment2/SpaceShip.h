#pragma once
#include "OBJGameObject.h"
class SpaceShip : public OBJGameObject
{
public:
	SpaceShip(std::string meshFilePath);

	void Update() override;

	void PollInput(unsigned char key, int x, int y) override;
	void KeyUp(unsigned char key, int x, int y);
};

