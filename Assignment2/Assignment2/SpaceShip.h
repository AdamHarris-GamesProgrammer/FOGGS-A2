#pragma once
#include "GameObject.h"
class SpaceShip : public GameObject
{
public:
	SpaceShip(std::string meshFilePath);

	void Update() override;
	void Render() override;

	void PollInput(unsigned char key, int x, int y) override;
	void KeyUp(unsigned char key, int x, int y);
};

