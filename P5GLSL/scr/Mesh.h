#pragma once
#include <vector>
#include <list>
#include "Object.h"


class Mesh
{
private:

	//Lista de objetos que usan esta malla
	std::list<Object*> objects;


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

	//TEXTURAS

	//Textura de color
	unsigned int colorTex = -1;

	//Textura especular
	unsigned int specTex = -1;

	//Textura emisiva
	unsigned int emiTex = -1;

	//Textura de normales
	unsigned int normTex = -1;

	//IDs VAO VBOs

	unsigned int vao;
	unsigned int posVBO;
	unsigned int colorVBO;
	unsigned int normalVBO;
	unsigned int texCoordVBO;
	unsigned int tangentVBO;
	unsigned int triangleIndexVBO;

	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;
	int inTangent;

	static unsigned char* loadTexture(const char* fileName, unsigned int &w, unsigned int &h);
	static unsigned int loadTex(const char* fileName);
	void generateVAO();


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

	//Getters de textura
	unsigned int getColorTex();
	unsigned int getSpecTex();
	unsigned int getEmiTex();
	unsigned int getNormTex();
	unsigned int getVAO();

	int getNumVertex();
	int getNumTriangles();

	//TODO: change texture management
	//Texture loaders
	bool loadColorTex(const char* file);
	bool loadSpecTex(const char* file);
	bool loadEmiTex(const char* file);
	bool loadNormTex(const char* file);

	//Object management
	void addObject(Object* o);
	void removeObject(Object* o);

	//Get objects list
	std::list<Object*>& getObjects();

	//Render all objects
	//void render(Camera &camera, Shader &shader);

	~Mesh();
};

