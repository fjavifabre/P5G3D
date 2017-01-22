#pragma once
#include <vector>

class Mesh
{
private:

	//Indices de los triangulos de la malla
	std::vector<unsigned int> triangleIndex;

	//Posiciones de los vertices
	std::vector<float> vertexPos;

	//Normales de los vertices
	std::vector<float> vertexNormal;

	//Color de los vertices
	std::vector<float> vertexColor;

	//Coordenada de textura de los vertices
	std::vector<float> vertexTexCoord;

	//Tangente de los vertices
	std::vector<float> vertexTangent;

	//Textura de color
	char* colorTex = nullptr;

	//Textura especular
	char* specTex = nullptr;

	//Textura emisiva
	char* emiTex = nullptr;

	//Textura de normales
	char* normTex = nullptr;


public:
	/* Construye la malla a partir de un fichero utilizando assimp*/
	Mesh(const char* file);

	/* Construye maalla a partir de arrays */
	Mesh(const unsigned int nTriangles, const unsigned int nVertex,
		const unsigned int* index, const float* pos, const float* n=nullptr, const float* color= nullptr, const float* tex = nullptr, const float* tangent= nullptr);

	//Getters (devuelven null si no esta)

	unsigned int* getTriangleIndex(); //indices de los triangulos
	float* getVertexPos(); //Posiciones de los vertices
	float* getVertexNormals(); //Normales
	float* getVertexColor(); //Colores
	float* getVertexTexCoord(); //Coordenadas de textura
	float* getVertexTangent(); //Tangentes

	int getNumVertex();
	int getNumTriangles();

	~Mesh();
};

