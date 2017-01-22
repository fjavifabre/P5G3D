#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h> 

//TODO mejorar, solo genera la primera malla del archivo
Mesh::Mesh(const char* file)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene)
		throw std::invalid_argument("Fichero invalido");


	//SOLO GENERA LA PRIMERA MALLA!!

	const int objectNVertex = scene->mMeshes[0]->mNumVertices; //Numero de vertices
	const int objectNTriangle = scene->mMeshes[0]->mNumFaces; //Numero de triangulos

															  //Indices y posiciones
	triangleIndex.resize(objectNTriangle * 3);
	vertexPos.resize(objectNVertex * 3);

	//Colores
	if (scene->mMeshes[0]->HasVertexColors(0))
		vertexColor.resize(objectNVertex * 3);
	else
		vertexColor.clear();

	//Normales
	if (scene->mMeshes[0]->HasNormals())
		vertexNormal.resize(objectNVertex * 3);
	else
		vertexNormal.clear();

	//Coordenadas de textura
	if (scene->mMeshes[0]->HasTextureCoords(0))
		vertexTexCoord.resize(objectNVertex * 2);
	else
		vertexTexCoord.clear();

	//Tangentes
	if (scene->mMeshes[0]->HasTangentsAndBitangents())
		vertexTangent.resize(objectNVertex * 3);
	else
		vertexTangent.clear();


	//Para cada triangulo de la malla
	for (unsigned int j = 0; j < objectNTriangle; j++)
	{
		triangleIndex[j * 3] = (unsigned int)scene->mMeshes[0]->mFaces[j].mIndices[0];
		triangleIndex[j * 3 + 1] = (unsigned int)scene->mMeshes[0]->mFaces[j].mIndices[1];
		triangleIndex[j * 3 + 2] = (unsigned int)scene->mMeshes[0]->mFaces[j].mIndices[2];

	}

	//para cada vertices de la malla
	for (unsigned int j = 0; j < objectNVertex; j++)
	{
		//Coordenadas
		vertexPos[j * 3] = scene->mMeshes[0]->mVertices[j].x;
		vertexPos[j * 3 + 1] = scene->mMeshes[0]->mVertices[j].y;
		vertexPos[j * 3 + 2] = scene->mMeshes[0]->mVertices[j].z;

		//Color
		if (scene->mMeshes[0]->HasVertexColors(0))
		{
			vertexColor[j * 3] = scene->mMeshes[0]->mColors[j]->r;
			vertexColor[j * 3 + 1] = scene->mMeshes[0]->mColors[j]->g;
			vertexColor[j * 3 + 2] = scene->mMeshes[0]->mColors[j]->b;
		}


		//Normales
		if (scene->mMeshes[0]->HasNormals())
		{
			vertexNormal[j * 3] = scene->mMeshes[0]->mNormals[j].x;
			vertexNormal[j * 3 + 1] = scene->mMeshes[0]->mNormals[j].y;
			vertexNormal[j * 3 + 2] = scene->mMeshes[0]->mNormals[j].z;
		}


		//Coordenadas de textura
		if (scene->mMeshes[0]->HasTextureCoords(0))
		{
			vertexTexCoord[j * 2] = scene->mMeshes[0]->mTextureCoords[0][j].x;
			vertexTexCoord[j * 2 + 1] = scene->mMeshes[0]->mTextureCoords[0][j].y;
		}

		//Tangentes
		if (scene->mMeshes[0]->HasTangentsAndBitangents())
		{
			vertexTangent[j * 3] = scene->mMeshes[0]->mTangents[j].x;
			vertexTangent[j * 3 + 1] = scene->mMeshes[0]->mTangents[j].y;
			vertexTangent[j * 3 + 2] = scene->mMeshes[0]->mTangents[j].z;
		}
	}


	//delete scene;
}

Mesh::Mesh(const unsigned int objectNTriangle, const unsigned int objectNVertex,
	const unsigned int* index, const float* pos, const float* n, const float* color, const float* tex, const float* tangent)
{

	//Indices y posiciones
	triangleIndex.resize(objectNTriangle * 3);
	vertexPos.resize(objectNVertex * 3);

	std::copy(index, index + objectNTriangle * 3, triangleIndex.begin());
	std::copy(pos, pos + objectNVertex * 3, vertexPos.begin());

	//Colores
	if (color != nullptr)
	{
		vertexColor.resize(objectNVertex * 3);
		std::copy(color, color + objectNVertex * 3, vertexColor.begin());
	}
		
	else
		vertexColor.clear();

	//Normales
	if (n != nullptr)
	{
		vertexNormal.resize(objectNVertex * 3);
		std::copy(n, n + objectNVertex * 3, vertexNormal.begin());
	}
		
	else
		vertexNormal.clear();

	//Coordenadas de textura
	if (tex != nullptr)
	{
		vertexTexCoord.resize(objectNVertex * 2);
		std::copy(tex, tex + objectNVertex * 2, vertexTexCoord.begin());
	}
		
	else
		vertexTexCoord.clear();

	//Tangentes
	if (tangent != nullptr)
	{
		vertexTangent.resize(objectNVertex * 3);
		std::copy(tangent, tangent + objectNVertex * 3, vertexTangent.begin());
	}
		
	else
		vertexTangent.clear();


	
}

 unsigned int* Mesh::getTriangleIndex()
{
	return &triangleIndex[0];
}

 float* Mesh::getVertexPos()
{
	return &vertexPos[0];
}

 float* Mesh::getVertexNormals()
{
	return &vertexNormal[0];
}

 float* Mesh::getVertexColor()
{
	return &vertexColor[0];
}

 float* Mesh::getVertexTexCoord()
{
	return &vertexTexCoord[0];
}

 float* Mesh::getVertexTangent()
{
	return &vertexTangent[0];
}

 int Mesh::getNumVertex()
{
	return vertexPos.size() / 3;
}

 int Mesh::getNumTriangles()
{
	return triangleIndex.size() / 3;
}


Mesh::~Mesh()
{
}
