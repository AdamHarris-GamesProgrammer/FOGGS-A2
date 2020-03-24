#pragma once
#include "GameObject.h"
#include "OBJLoader.h"
#include "GraphicsStructures.h"
#include "MathStructures.h"

#include "GLIncludes.h"

class OBJGameObject : public GameObject
{
public:
	OBJGameObject(std::string meshFilePath);

	void Render() override;
private:


protected:
	std::vector<Mesh> mObjectMesh;
	OBJLoader* mMeshLoader;
};

