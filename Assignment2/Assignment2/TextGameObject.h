#pragma once

#include "GameObject.h"
#include "TextMeshLoader.h"

class TextGameObject : public GameObject
{
public:
	TextGameObject(std::string meshFilePath);

	void Render() override;
	
private:

protected:
	TextMesh* mMesh;
	Material* mMaterial;
};

