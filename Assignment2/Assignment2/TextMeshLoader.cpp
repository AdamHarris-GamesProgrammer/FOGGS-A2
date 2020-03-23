#include "TextMeshLoader.h"

#include <iostream>
#include <fstream>


using namespace std;

namespace TextMeshLoader {
	void LoadVertices(ifstream& inFile, TextMesh& mesh);
	void LoadIndices(ifstream& inFile, TextMesh& mesh);

	void LoadVertices(ifstream& inFile, TextMesh& mesh)
	{
		inFile >> mesh.VertexCount;

		if (mesh.VertexCount > 0)
		{
			mesh.Vertices = new Vector3[mesh.VertexCount];

			for (int i = 0; i < mesh.VertexCount; i++)
			{
				inFile >> mesh.Vertices[i].x;
				inFile >> mesh.Vertices[i].y;
				inFile >> mesh.Vertices[i].z;
			}
		}
	}

	void LoadTextureCoords(ifstream& inFile, TextMesh& mesh) {
		inFile >> mesh.TexCoordCount;
		mesh.TexCoords = new Vector2[mesh.TexCoordCount];
		for (int i = 0; i < mesh.TexCoordCount; i++) {
			inFile >> mesh.TexCoords[i].x;
			inFile >> mesh.TexCoords[i].y;
		}
	}

	void LoadIndices(ifstream& inFile, TextMesh& mesh)
	{
		inFile >> mesh.IndexCount;
		mesh.Indices = new GLushort[mesh.IndexCount];
		for (int i = 0; i < mesh.IndexCount; i++) {
			inFile >> mesh.Indices[i];
		}
	}

	void LoadNormals(ifstream& inFile, TextMesh& mesh) {
		inFile >> mesh.NormalCount;
		mesh.Normals = new Vector3[mesh.NormalCount];
		for (int i = 0; i < mesh.NormalCount; i++) {
			inFile >> mesh.Normals[i].x;
			inFile >> mesh.Normals[i].y;
			inFile >> mesh.Normals[i].z;
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