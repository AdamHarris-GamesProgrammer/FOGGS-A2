#pragma once
#include "TextGameObject.h"
class Coin : public TextGameObject
{
public:
	Coin(std::string meshFilePath);

	void Update() override;

	void GeneratePosition();


private:

};

