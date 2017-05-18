#pragma once
#include <vector>
#include <list>


#include <gl/glew.h> //Siempre antes que GL

#include <glm\glm.hpp>


namespace Taranis
{

	enum TOPOLOGY
	{
		POINTS,
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
	};


	//!  Mesh class. 
	/*!
	Class used to define and control a mesh. Contains functions related with its points, textures and properties.
	*/
	class Mesh
	{



		//! List of objects using this mesh.
		/*!
		The objects using this mesh could implement different sizes, scales and positions of a single mesh.
		*/

		/*
		std::list<Object*> objects;

		*/

	protected:

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

	public:
		TOPOLOGY Topology;

	private:

		//! Color texture ID
		unsigned int colorTex = 0;

		//! Specular texture ID
		unsigned int specTex = 0;

		//! Emissive texture ID
		unsigned int emiTex = 0;

		//! Normal texture ID
		unsigned int normTex = 0;

		//IDs VAO VBOs

		unsigned int m_VAO = 0; //!< VAO ID of this mesh

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


		void Loader(std::string fileName, bool generateNormals, bool generateTangents);


	public:


		Mesh();

		//! Class constructor.
		/*!
		Mesh class constructor.

		\param file File where the mesh is stored.
		*/
		Mesh(const char* file);

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
		/*Mesh(const unsigned int nTriangles, const unsigned int nVertex,
			const unsigned int* index, const float* pos, Shader * material, const float* n = nullptr, const float* color = nullptr, const float* tex = nullptr, const float* tangent = nullptr);*/



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

		


		//! Mesh class destructor.
		/*!
		Frees all memory used by all the textures and buffers in this mesh.
		*/
		~Mesh();



		void Generate(bool interleaved = true);
	};


}

