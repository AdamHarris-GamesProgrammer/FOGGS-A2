#include "TextMeshLoader.h"

#include <iostream>
#include <fstream>


using namespace std;

namespace TextMeshLoader {
	void LoadVertices(ifstream& inFile, TextMesh& mesh);
	void LoadIndices(ifstream& inFile, TextMesh& mesh);

	void LoadVertices(ifstream& inFile, TextMesh& mesh)
	{
		inFile >> mesh.vertexCount;

		if (mesh.vertexCount > 0)
		{
			mesh.vertices = new Vector3[mesh.vertexCount];

			for (int i = 0; i < mesh.vertexCount; i++)
			{
				inFile >> mesh.vertices[i].x;
				inFile >> mesh.vertices[i].y;
				inFile >> mesh.vertices[i].z;
			}
		}
	}

	void LoadTextureCoords(ifstream& inFile, TextMesh& mesh) {
		inFile >> mesh.texCoordCount;
		mesh.texCoords = new Vector2[mesh.texCoordCount];
		for (int i = 0; i < mesh.texCoordCount; i++) {
			inFile >> mesh.texCoords[i].x;
			inFile >> mesh.texCoords[i].y;
		}
	}

	void LoadIndices(ifstream& inFile, TextMesh& mesh)
	{
		inFile >> mesh.indexCount;
		mesh.indices = new GLushort[mesh.indexCount];
		for (int i = 0; i < mesh.indexCount; i++) {
			inFile >> mesh.indices[i];
		}
	}

	void LoadNormals(ifstream& inFile, TextMesh& mesh) {
		inFile >> mesh.normalCount;
		mesh.normals = new Vector3[mesh.normalCount];
		for (int i = 0; i < mesh.normalCount; i++) {
			inFile >> mesh.normals[i].x;
			inFile >> mesh.normals[i].y;
			inFile >> mesh.normals[i].z;
		}
	}

	TextMesh* TextMeshLoader::Load(const char* path)
	{
		TextMesh* mesh = new TextMesh();

		ifstream inFile;

		inFile.open(path);

		if (!inFile.good())
		{
			cerr << "Can't open TextMesh file " << path << endl;
			return nullptr;
		}


		LoadVertices(inFile, *mesh);
		LoadTextureCoords(inFile, *mesh);
		LoadNormals(inFile, *mesh);
		LoadIndices(inFile, *mesh);

		inFile.close();

		return mesh;
	}
}