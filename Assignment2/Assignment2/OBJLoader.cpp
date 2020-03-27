#include "OBJLoader.h"

OBJLoader::OBJLoader()
{

}

OBJLoader::~OBJLoader()
{
	mLoadedMeshes.clear();
	mLoadedVerticies.clear();
	mLoadedIndices.clear();
	mLoadedMaterial.clear();
}

bool OBJLoader::LoadFile(std::string path)
{
	if (path.substr(path.size() - 4, 4) != ".obj") {
		std::cout << "Invalid File Format!" << std::endl;
		return false;
	}

	std::ifstream file(path);

	if (!file.is_open()) {
		return false;
	}

	mLoadedMeshes.clear();
	mLoadedVerticies.clear();
	mLoadedIndices.clear();

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;

	std::vector<Vector2> texCoords;

	std::vector<Vertex> verticies;
	std::vector<unsigned int> indices;

	std::vector <std::string> meshMaterialNames;

	bool listening = false;
	std::string meshName;

	Mesh tempMesh;

	std::string currentLine;
	while (std::getline(file, currentLine)) {
		if (Algorithm::FirstToken(currentLine) == "o" || Algorithm::FirstToken(currentLine) == "g" || currentLine[0] == 'g') {
			if (!listening) {
				listening = true;
				meshName = LoadObjectName(currentLine);
			}
			else {
				if (!mLoadedIndices.empty() && !mLoadedVerticies.empty()) {
					tempMesh = Mesh(mLoadedVerticies, mLoadedIndices);
					tempMesh.meshName = meshName;

					mLoadedMeshes.push_back(tempMesh);

					verticies.clear();
					indices.clear();
					meshName.clear();

					meshName = Algorithm::Tail(currentLine);
				}
				else
				{
					meshName = LoadObjectName(currentLine);
				}
			}
		}

		//Generate a vertex position
		else if (Algorithm::FirstToken(currentLine) == "v") {
			LoadVertices(positions, currentLine);
		}

		//Generate a vertex texture coordinate
		else if (Algorithm::FirstToken(currentLine) == "vt") {
			LoadVertexTextures(texCoords, currentLine);
		}

		//Generates vertex normal coordinates
		else if (Algorithm::FirstToken(currentLine) == "vn") {
			LoadVertexNormals(normals, currentLine);
		}

		//Generates a face from verticies and indices
		else if (Algorithm::FirstToken(currentLine) == "f") {
			LoadFaces(verticies, indices,positions, texCoords, normals, currentLine);
		}

		//Gets the material name (if applicable)
		else if (Algorithm::FirstToken(currentLine) == "usemtl") {
			GetMaterialName(tempMesh, meshName, meshMaterialNames, verticies, indices, currentLine);
		}

		//Load Materials
		else if (Algorithm::FirstToken(currentLine) == "mtllib") {
			LoadMaterials(GetMaterialFilePath(path,currentLine));
		}
	}

	//Deals with remaining vertices and indices
	if (!indices.empty() && !verticies.empty()) {
		tempMesh = Mesh(verticies, indices);
		tempMesh.meshName = meshName;

		mLoadedMeshes.push_back(tempMesh);
	}

	file.close();

	//Set materials for each mesh
	for (int i = 0; i < meshMaterialNames.size(); i++) {
		std::string matname = meshMaterialNames[i];

		for (int j = 0; j < mLoadedMaterial.size(); j++) {
			if (mLoadedMaterial[j].name == matname) {
				mLoadedMeshes[i].meshMaterial = mLoadedMaterial[j];
				break;
			}
		}
	}

	if (mLoadedMeshes.empty() && mLoadedVerticies.empty() && mLoadedIndices.empty()) {
		return false;
	}
	else
	{
		return true;
	}
}

void OBJLoader::LoadVertices(std::vector<Vector3>& inPositions, std::string& currentLine)
{
	std::vector<std::string> spos;
	Vector3 vpos;
	Algorithm::Split(Algorithm::Tail(currentLine), spos, " ");

	vpos.x = std::stof(spos[0]);
	vpos.y = std::stof(spos[1]);
	vpos.z = std::stof(spos[2]);

	inPositions.push_back(vpos);
}

void OBJLoader::LoadVertexTextures(std::vector<Vector2>& inTexCoords, std::string& currentLine)
{
	std::vector<std::string> stex;
	Vector2 vtex;
	Algorithm::Split(Algorithm::Tail(currentLine), stex, " ");

	vtex.x = std::stof(stex[0]);
	vtex.y = std::stof(stex[1]);

	inTexCoords.push_back(vtex);
}

void OBJLoader::LoadVertexNormals(std::vector<Vector3>& inNormals, std::string& currentLine)
{
	std::vector<std::string> snor;
	Vector3 vnor;
	Algorithm::Split(Algorithm::Tail(currentLine), snor, " ");

	vnor.x = std::stof(snor[0]);
	vnor.y = std::stof(snor[1]);
	vnor.z = std::stof(snor[2]);

	inNormals.push_back(vnor);
}

void OBJLoader::LoadFaces(std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::vector<Vector3>& inPositions, std::vector<Vector2>& inTexCoords, std::vector<Vector3>& inNormals, std::string& inCurrentLine)
{
	// Generate the vertices
	std::vector<Vertex> vVerts;
	GenerateVerticiesFromRawOBJ(vVerts, inPositions, inTexCoords, inNormals, inCurrentLine);

	// Add Vertices
	for (int i = 0; i < int(vVerts.size()); i++)
	{
		inVertices.push_back(vVerts[i]);

		mLoadedVerticies.push_back(vVerts[i]);
	}

	std::vector<unsigned int> iIndices;

	VertexTriangulation(iIndices, vVerts);

	// Add Indices
	for (int i = 0; i < int(iIndices.size()); i++)
	{
		unsigned int indnum = (unsigned int)((inVertices.size()) - vVerts.size()) + iIndices[i];
		inIndices.push_back(indnum);

		indnum = (unsigned int)((mLoadedVerticies.size()) - vVerts.size()) + iIndices[i];
		mLoadedIndices.push_back(indnum);

	}
}

void OBJLoader::GenerateVerticiesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string inLine)
{
	std::vector<std::string> sface, svert;
	Vertex vVert;
	Algorithm::Split(Algorithm::Tail(inLine), sface, " ");

	bool noNormal = false;

	//for every vertex do this
	for (int i = 0; i < int(sface.size()); i++) {
		int vType;

		Algorithm::Split(sface[i], svert, "/");

		//Checks for just position (v1)
		if (svert.size() == 1) {
			vType = 1;
		}

		//Checks for position and texture (v1/vt1)
		if (svert.size() == 2) {
			vType = 2;
		}

		//Checks for position, texture and normal (v1,vt1,vn1) or if just position and normal
		if (svert.size() == 3) {
			if (svert[1] != "") {
				//position texture and normal
				vType = 4;
			}
			else
			{
				//position and normal
				vType = 3;
			}
		}

		switch (vType)
		{
		case 1: //Just position
		{
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Vector2();
			noNormal = true;
			outVerts.push_back(vVert);
			break;
		}
		case 2: //Position and texture
		{
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Algorithm::GetElement(inTexCoords, svert[1]);
			noNormal = true;
			outVerts.push_back(vVert);
			break;
		}
		case 3: //Position and normals
		{
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Vector2();
			vVert.normal = Algorithm::GetElement(inNormals, svert[2]);
			outVerts.push_back(vVert);
			break;
		}
		case 4: //Position, texture and normals
		{
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Algorithm::GetElement(inTexCoords, svert[1]);
			vVert.normal = Algorithm::GetElement(inNormals, svert[2]);
			outVerts.push_back(vVert);
			break;
		}
		default:
			break;
		}


		//Generates a normal if no normal is found
		if (noNormal) {
			Vector3 A = outVerts[0].position - outVerts[1].position;
			Vector3 B = outVerts[2].position - outVerts[1].position;

			Vector3 normal = Math::CrossProduct(A, B);

			for (int i = 0; i < int(outVerts.size()); i++) {
				outVerts[i].normal = normal;
			}
		}
	}
}

void OBJLoader::VertexTriangulation(std::vector<unsigned int>& outIndices, const std::vector<Vertex>& inVerts)
{
	// If there are 2 or less verts,
	// no triangle can be created,
	// so exit
	if (inVerts.size() < 3)
	{
		return;
	}
	// If it is a triangle no need to calculate it
	if (inVerts.size() == 3)
	{
		outIndices.push_back(0);
		outIndices.push_back(1);
		outIndices.push_back(2);
		return;
	}

	// Create a list of vertices
	std::vector<Vertex> tVerts = inVerts;

	while (true)
	{
		// For every vertex
		for (int i = 0; i < int(tVerts.size()); i++)
		{
			// pPrev = the previous vertex in the list
			Vertex pPrev;
			if (i == 0)
			{
				pPrev = tVerts[tVerts.size() - 1];
			}
			else
			{
				pPrev = tVerts[i - 1];
			}

			// pCur = the current vertex;
			Vertex pCur = tVerts[i];

			// pNext = the next vertex in the list
			Vertex pNext;
			if (i == tVerts.size() - 1)
			{
				pNext = tVerts[0];
			}
			else
			{
				pNext = tVerts[i + 1];
			}

			// Check to see if there are only 3 verts left
			// if so this is the last triangle
			if (tVerts.size() == 3)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (inVerts[j].position == pCur.position)
						outIndices.push_back(j);
					if (inVerts[j].position == pPrev.position)
						outIndices.push_back(j);
					if (inVerts[j].position == pNext.position)
						outIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}
			if (tVerts.size() == 4)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(inVerts.size()); j++)
				{
					if (inVerts[j].position == pCur.position)
						outIndices.push_back(j);
					if (inVerts[j].position == pPrev.position)
						outIndices.push_back(j);
					if (inVerts[j].position == pNext.position)
						outIndices.push_back(j);
				}

				Vector3 tempVec;
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].position != pCur.position
						&& tVerts[j].position != pPrev.position
						&& tVerts[j].position != pNext.position)
					{
						tempVec = tVerts[j].position;
						break;
					}
				}

				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(inVerts.size()); j++)
				{
					if (inVerts[j].position == pPrev.position)
						outIndices.push_back(j);
					if (inVerts[j].position == pNext.position)
						outIndices.push_back(j);
					if (inVerts[j].position == tempVec)
						outIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			// If Vertex is not an interior vertex
			float angle = Math::AngleBetweenVectors(pPrev.position - pCur.position, pNext.position - pCur.position) * (180 / 3.14159265359);
			if (angle <= 0 && angle >= 180)
				continue;

			// If any vertices are within this triangle
			bool inTri = false;
			for (int j = 0; j < int(inVerts.size()); j++)
			{
				if (Algorithm::InTriangle(inVerts[j].position, pPrev.position, pCur.position, pNext.position)
					&& inVerts[j].position != pPrev.position
					&& inVerts[j].position != pCur.position
					&& inVerts[j].position != pNext.position)
				{
					inTri = true;
					break;
				}
			}
			if (inTri)
				continue;

			// Create a triangle from pCur, pPrev, pNext
			for (int j = 0; j < int(inVerts.size()); j++)
			{
				if (inVerts[j].position == pCur.position)
					outIndices.push_back(j);
				if (inVerts[j].position == pPrev.position)
					outIndices.push_back(j);
				if (inVerts[j].position == pNext.position)
					outIndices.push_back(j);
			}

			// Delete pCur from the list
			for (int j = 0; j < int(tVerts.size()); j++)
			{
				if (tVerts[j].position == pCur.position)
				{
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

			// reset i to the start
			// -1 since loop will add 1 to it
			i = -1;
		}

		// if no triangles were created
		if (outIndices.size() == 0)
			break;

		// if no more vertices
		if (tVerts.size() == 0)
			break;
	}
}

void OBJLoader::GetMaterialName(Mesh& inTempMesh, std::string& inMeshName, std::vector<std::string>& inMeshMaterialNames, std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::string& inCurrentLine)
{
	inMeshMaterialNames.push_back(Algorithm::Tail(inCurrentLine));

	if (!inIndices.empty() && !inVertices.empty()) {
		inTempMesh = Mesh(inVertices, inIndices);
		inTempMesh.meshName = inMeshName;
		int i = 2;
		while (1) {
			inTempMesh.meshName = inMeshName + "_" + std::to_string(i);

			for (auto& m : mLoadedMeshes) {
				if (m.meshName == inTempMesh.meshName) {
					continue;
				}
			}
			break;
		}

		mLoadedMeshes.push_back(inTempMesh);

		//Cleanup 
		inVertices.clear();
		inIndices.clear();
	}
}

std::string OBJLoader::GetMaterialFilePath(std::string& inPath, std::string& inCurrentLine)
{
	std::vector <std::string> temp;
	Algorithm::Split(inPath, temp, "/");

	std::string pathToMaterial = "";

	if (temp.size() != 1) {
		for (int i = 0; i < temp.size() - 1; i++) {
			pathToMaterial += temp[i] + "/";
		}
	}

	return pathToMaterial += Algorithm::Tail(inCurrentLine);
}

bool OBJLoader::LoadMaterials(std::string path)
{
	if (path.substr(path.size() - 4, path.size()) != ".mtl") {
		std::cout << "Wrong file format" << std::endl;
		return false;
	}

	std::ifstream file(path);

	if (!file.is_open()) {
		return false;
	}

	Material tempMaterial;

	bool listening = false;

	std::string currentLine;

	while (std::getline(file, currentLine)) {
		//Finds the material name
		if (Algorithm::FirstToken(currentLine) == "newmtl") {
			if (!listening) {
				listening = true;

				if (currentLine.size() > 7) {
					tempMaterial.name = Algorithm::Tail(currentLine);
				}
				else
				{
					tempMaterial.name = "none";
				}
			}
			else
			{
				mLoadedMaterial.push_back(tempMaterial);

				tempMaterial = Material();

				if (currentLine.size() > 7) {
					tempMaterial.name = Algorithm::Tail(currentLine);
				}
				else
				{
					tempMaterial.name = "none";
				}
			}
		}

		//Ambient Colour
		else if (Algorithm::FirstToken(currentLine) == "Ka") {
			LoadColourAmbient(tempMaterial, currentLine);
		}

		//Diffuse Colour
		else if (Algorithm::FirstToken(currentLine) == "Kd") {
			LoadColourDiffuse(tempMaterial, currentLine);
		}

		//Specular colour
		else if (Algorithm::FirstToken(currentLine) == "Ks") {
			LoadColourSpecular(tempMaterial, currentLine);
		}

		//Specular exponent
		else if (Algorithm::FirstToken(currentLine) == "Ns") {
			tempMaterial.Ns = std::stof(Algorithm::Tail(currentLine));
		}
		//Anisotropic filtering value
		else if (Algorithm::FirstToken(currentLine) == "Ni") {
			tempMaterial.Ni = std::stof(Algorithm::Tail(currentLine));
		}
		//Dissolved value (Transparency)
		else if (Algorithm::FirstToken(currentLine) == "d") {
			tempMaterial.d = std::stof(Algorithm::Tail(currentLine));
		}
		//Illumination Value
		else if (Algorithm::FirstToken(currentLine) == "illum") {
			tempMaterial.illum = std::stof(Algorithm::Tail(currentLine));
		}
		//Ambient Map
		else if (Algorithm::FirstToken(currentLine) == "map_Ka") {
			tempMaterial.map_Ka = Algorithm::Tail(currentLine);
		}
		//Diffuse Map
		else if (Algorithm::FirstToken(currentLine) == "map_Kd") {
			tempMaterial.map_Kd = Algorithm::Tail(currentLine);
		}
		//Specular Map
		else if (Algorithm::FirstToken(currentLine) == "map_Ks") {
			tempMaterial.map_Ks = Algorithm::Tail(currentLine);
		}

		//Specular Exponent Map
		else if (Algorithm::FirstToken(currentLine) == "map_Ns") {
			tempMaterial.map_Ns = Algorithm::Tail(currentLine);
		}

		//Diffuse Map
		else if (Algorithm::FirstToken(currentLine) == "map_D") {
			tempMaterial.map_d = Algorithm::Tail(currentLine);
		}

		//Bump Map
		else if (Algorithm::FirstToken(currentLine) == "map_Bump" || Algorithm::FirstToken(currentLine) == "map_bump" || Algorithm::FirstToken(currentLine) == "bump") {
			tempMaterial.map_bump = Algorithm::Tail(currentLine);
		}
	}

	mLoadedMaterial.push_back(tempMaterial);

	if (mLoadedMaterial.empty()) {
		return false;
	}
	else
	{
		return true;
	}
}

std::string OBJLoader::LoadObjectName(std::string& currentLine)
{
	std::string meshName;

	if (Algorithm::FirstToken(currentLine) == "o" || Algorithm::FirstToken(currentLine) == "g") {
		meshName = Algorithm::Tail(currentLine);
	}
	else
	{
		meshName = "unnamed";
	}

	return meshName;
}

void OBJLoader::LoadColourAmbient(Material& inMaterial, std::string& inCurrentLine)
{
	std::vector<std::string> temp;
	Algorithm::Split(Algorithm::Tail(inCurrentLine), temp, " ");

	if (temp.size() != 3) {
		return;
	}

	inMaterial.Ka.x = std::stof(temp[0]);
	inMaterial.Ka.y = std::stof(temp[1]);
	inMaterial.Ka.z = std::stof(temp[2]);
}

void OBJLoader::LoadColourDiffuse(Material& inMaterial, std::string& inCurrentLine)
{
	std::vector<std::string> temp;
	Algorithm::Split(Algorithm::Tail(inCurrentLine), temp, " ");

	if (temp.size() != 3) {
		return;
	}

	inMaterial.Kd.x = std::stof(temp[0]);
	inMaterial.Kd.y = std::stof(temp[1]);
	inMaterial.Kd.z = std::stof(temp[2]);
}

void OBJLoader::LoadColourSpecular(Material& inMaterial, std::string& inCurrentLine)
{
	std::vector<std::string> temp;
	Algorithm::Split(Algorithm::Tail(inCurrentLine), temp, " ");

	if (temp.size() != 3) {
		return;
	}

	inMaterial.Ks.x = std::stof(temp[0]);
	inMaterial.Ks.y = std::stof(temp[1]);
	inMaterial.Ks.z = std::stof(temp[2]);
}