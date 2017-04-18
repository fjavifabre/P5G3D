#pragma once
#include <vector>
#include <list>
#include "Object.h"

class Shader;


//!  Mesh class. 
/*!
Class used to define and control a mesh. Contains functions related with its points, textures and properties.
*/
class Mesh
{
private:

	//! List of objects using this mesh.
	/*!
	The objects using this mesh could implement different sizes, scales and positions of a single mesh.
	*/
	std::list<Object*> objects;


	//! Index of the triangles in the mesh
	std::vector<unsigned int> triangleIndex;

	//! Position of the vertices in the mesh
	std::vector<glm::vec3> vertexPos;

	//! Normal vectors of the vertices in the mesh
	std::vector<glm::vec3> vertexNormal;

	//! Color of the vertices in the mesh
	std::vector<glm::vec3> vertexColor;

	//! Texture coordinates of the vertices in the mesh
	std::vector<glm::vec2> vertexTexCoord;

	//! Tangent vector of the vertices in the mesh
	std::vector<glm::vec3> vertexTangent;

	//! Bitanget vector of the vectices in the mesh
	std::vector<glm::vec3> vertexBitangent;


	//! Color texture ID
	unsigned int colorTex = 0;

	//! Specular texture ID
	unsigned int specTex = 0;

	//! Emissive texture ID
	unsigned int emiTex = 0;

	//! Normal texture ID
	unsigned int normTex = 0;

	//IDs VAO VBOs

	unsigned int m_VAO; //!< VAO ID of this mesh

	//TODO well try to use one only VBO

	unsigned int m_VBO; //!< VBO ID of this mesh

	/*
	unsigned int posVBO; //!< Position VBO ID of this mesh
	unsigned int colorVBO; //!< Color VBO ID of this mesh
	unsigned int normalVBO; //!< Normal VBO ID of this mesh
	unsigned int texCoordVBO; //!< Texture coordinates VBO ID of this mesh
	unsigned int tangentVBO; //!< Tangent vectors VBO ID of this mesh
	unsigned int triangleIndexVBO; //!< Triangle VBO ID of this mesh
	*/

	//! Shader in charge of rendering this mesh
	Shader* mat;

	//! Auxliar function to load textures from file.
	/*!
	\return Returns the array of pixels of the texture loaded.

	\param fileName File to load as texture.
	\param w Output width of the texture loaded.
	\param h Output height of the texture loaded.
	*/
	static unsigned char* loadTexture(const char* fileName, unsigned int &w, unsigned int &h);

	//! Auxliar function to load textures from file to the OpenGL context.
	/*!
	Returns the ID of the texture loaded. 0 if texture loading fails.

	\param fileName File to load as texture.
	*/
	static unsigned int loadTex(const char* fileName);

	//! Auxiliar function to generate VAO and VBO to manage this mesh.
	/*!
	Initialize all VBOs and VAO requiered to manage this mesh.
	*/
	void generateVAO();

	void Loader(std::string fileName, bool generateNormals, bool generateTangents);


public:
	//! Class constructor.
	/*!
	Mesh class constructor.

	\param file File where the mesh is stored.
	\param material Shader that will render the mesh.
	*/
	Mesh(const char* file, Shader * material);

	//! Class constructor.
	/*!
	Mesh class constructor.

	\param nTriangles Number of triangles in the mesh.
	\param nVertex Number of vertex in the mesh.
	\param index Array of vertex index.
	\param pos Array of vertex positions.
	\param material Shader that will render the mesh.
	\param n Array of vertex normals.
	\param tex Array of vertex texture coordinates.
	\param tangent Array of vertex tangents.
	*/
	Mesh(const unsigned int nTriangles, const unsigned int nVertex,
		const unsigned int* index, const float* pos, Shader * material, const float* n=nullptr, const float* color= nullptr, const float* tex = nullptr, const float* tangent= nullptr);

	//! Function to set a shader in charge of render this mesh.
	/*!
	Sets the shader used to render this mesh.

	\param material Shader to use.
	*/
	void ApplyMaterial(Shader * material);

	//Getters (devuelven null si no esta)

	//! Triangles index getter.
	/*!
	\return A pointer to the triangles index array.
	*/
	unsigned int* getTriangleIndex();

	//! Vertices position getter.
	/*!
	\return A pointer to the vertices position array.
	*/
	float* getVertexPos();

	//! Vertices normal getter.
	/*!
	\return A pointer to the vertices normal array.
	*/
	float* getVertexNormals();

	//! Vertices color getter.
	/*!
	\return A pointer to the vertices normal array.
	*/
	float* getVertexColor();

	//! Vertices texture coordinates getter.
	/*!
	\return A pointer to the vertices texture coordinates array.
	*/
	float* getVertexTexCoord();

	//! Vertices tangent getter.
	/*!
	\return A pointer to the vertices tangent array.
	*/
	float* getVertexTangent();

	//Getters de textura

	//! Color texture getter.
	/*!
	\return The color texture ID.
	*/
	unsigned int getColorTex();

	//! Specular texture getter.
	/*!
	\return The specular texture ID.
	*/
	unsigned int getSpecTex();

	//! Emissive texture getter.
	/*!
	\return The emissive texture ID.
	*/
	unsigned int getEmiTex();

	//! Normal texture getter.
	/*!
	\return The normal texture ID.
	*/
	unsigned int getNormTex();

	//! VAO ID getter.
	/*!
	\return VAO ID.
	*/
	unsigned int getVAO();

	//! Number of vertex getter.
	/*!
	\return The number of vertex in the mesh.
	*/
	int getNumVertex();

	//! Number of triangles.
	/*!
	\return The number of triangles in the mesh.
	*/
	int getNumTriangles();

	//TODO: change texture management
	//Texture loaders

	//! Color texture loader.
	/*!
	\param file File to load as texture.
	\return True if the texture loaded successfully, otherwise false .
	*/
	bool loadColorTex(const char* file);

	//! Specular texture loader.
	/*!
	\param file File to load as texture.
	\return True if the texture loaded successfully, otherwise false .
	*/
	bool loadSpecTex(const char* file);

	//! Emissive texture loader.
	/*!
	\param file File to load as texture.
	\return True if the texture loaded successfully, otherwise false .
	*/
	bool loadEmiTex(const char* file);

	//! Normal texture loader.
	/*!
	\param file File to load as texture.
	\return True if the texture loaded successfully, otherwise false .
	*/
	bool loadNormTex(const char* file);

	//Object management

	//! Function to add object so they use this mesh.
	/*!
	\param o Object to add.
	Sets a object to use this mesh.
	*/
	void addObject(Object* o);

	//! Function to remove using this mesh.
	/*!
	\param o Object to remove.
	*/
	void removeObject(Object* o);

	//! Function to obtain the list of Objects using this mesh.
	/*!
	\return A list with pointers to all objects using this mesh.
	*/
	std::list<Object*>& getObjects();

	
	//! Mesh class destructor.
	/*!
	Frees all memory used by all the textures and buffers in this mesh.
	*/
	~Mesh();



	//TEST
	void Generate(bool interleaved=true);
};

