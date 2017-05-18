#include "mesh/mesh.h"


//#include <gl/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>

//Carga de texturas
//#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

#define MAX 100

namespace Taranis
{
	void updateBar(int pos, int length, float lastProg, int last)
	{
		//Print progress
		float prog(pos / float(length)); // percentage of infile already read

		if (pos == -1)
		{
			int cur = MAX + 1;

			std::cout << std::fixed << std::setprecision(2)
				<< "\r   [" << std::string(cur, '#')
				<< std::string(MAX + 1 - cur, ' ') << "] " << 100 * prog << "%";

			if (prog == 1) std::cout << std::endl;
			else std::cout.flush();

			return;
		}

		// Number of #'s as function of current progress "prog"
		int cur(std::ceil(prog * MAX));
		if (last != cur)
		{
			last = cur;
			std::cout << std::fixed << std::setprecision(2)
				<< "\r   [" << std::string(cur, '#')
				<< std::string(MAX + 1 - cur, ' ') << "] " << 100 * prog << "%";

			if (prog == 1) std::cout << std::endl;
			else std::cout.flush();
		}

		std::cout << std::fixed << std::setprecision(2)
			<< "\r   [" << std::string(cur, '#')
			<< std::string(MAX + 1 - cur, ' ') << "] " << 100 * prog << "%";

		if (prog == 1) std::cout << std::endl;
		else std::cout.flush();

	}


	void Mesh::Loader(std::string fileName, bool generateNormals = false, bool generateTangets = false)
	{


		float lastProg = 0.0f;

		float x, y, z;

		int last = 0;

		std::ifstream is;
		is.open(fileName);
		if (is.fail()) throw "Unable to open " + fileName;

		//Progress bar
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);



		std::cout << "Loading " << fileName << std::endl;
		std::cout.flush();

		std::string lineString;
		float lastTime = std::clock();
		while (std::getline(is, lineString))
		{
			std::istringstream line(lineString);

			std::string prefix; //Line prefix
			line >> prefix;



			if (prefix == "v") //Vertex
			{
				glm::vec3 v;
				line >> v.x >> v.y >> v.z;

				vertexPos.push_back(v);

			}

			else if (prefix == "vt") //Tex Coord
			{
				glm::vec2 vt;
				line >> vt.x >> vt.y;

				vertexTexCoord.push_back(vt);
			}

			else if (prefix == "vn") //Normal
			{
				glm::vec3 vn;
				line >> vn.x >> vn.y >> vn.z;

				vertexNormal.push_back(vn);
			}

			else if (prefix == "f") //Face
			{
				//Update

				unsigned int v1, v2, v3, v4 = -1;
				line >> v1 >> v2 >> v3 >> v4;

				triangleIndex.push_back(v1-1);
				triangleIndex.push_back(v2-1);
				triangleIndex.push_back(v3-1);


				//Quad --> split second triangle
				if (v4 != -1)
				{
					triangleIndex.push_back(v4-1);
					triangleIndex.push_back(v1-1);
					triangleIndex.push_back(v3-1);
				}

			}
			if ((std::clock() - lastTime) / CLOCKS_PER_SEC > 2)
			{
				updateBar((int)is.tellg(), length, lastProg, last);
				lastTime = std::clock();
			}





		}
		updateBar((int)is.tellg(), length, lastProg, last);
		std::cout << std::endl;

		//Normals TODO:maybe faster imp
		if (generateNormals && vertexNormal.size() == 0)
		{
			//Initialize normals to 0s
			vertexNormal.resize(vertexPos.size(), glm::vec3(0.0f));

			//Cross
			for (int i = 0; i < triangleIndex.size(); i += 3)
			{
				glm::vec3 A = vertexPos[triangleIndex[i]];
				glm::vec3 B = vertexPos[triangleIndex[i + 1]];
				glm::vec3 C = vertexPos[triangleIndex[i + 2]];

				glm::vec3 p = glm::cross(B - A, C - A);

				vertexNormal[triangleIndex[i]] += p;
				vertexNormal[triangleIndex[i + 1]] += p;
				vertexNormal[triangleIndex[i + 2]] += p;

			}

			for (int i = 0; i < vertexNormal.size(); i++)
				vertexNormal[i] = glm::normalize(vertexNormal[i]);

		}

		//Tangents
		if (generateTangets && vertexTangent.size() == 0 && vertexTexCoord.size() != 0)
		{
			for (int i = 0; i < vertexPos.size(); i += 3)
			{
				// vertices
				glm::vec3& v0 = vertexPos[i];
				glm::vec3& v1 = vertexPos[i + 1];
				glm::vec3& v2 = vertexPos[i + 2];

				//UVs
				glm::vec2& uv0 = vertexTexCoord[i];
				glm::vec2& uv1 = vertexTexCoord[i + 1];
				glm::vec2& uv2 = vertexTexCoord[i + 2];

				// Edges of the triangle
				glm::vec3 deltaPos1 = v1 - v0;
				glm::vec3 deltaPos2 = v2 - v0;

				// UV delta
				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				// Compute tangents and bitangents
				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;


				vertexTangent.push_back(tangent);
				vertexTangent.push_back(tangent);
				vertexTangent.push_back(tangent);

				vertexBitangent.push_back(bitangent);
				vertexBitangent.push_back(bitangent);
				vertexBitangent.push_back(bitangent);


			}
		}



	}





	Mesh::Mesh() {}

	Mesh::Mesh(const char* file)
	{

		Loader(std::string(file), true, true);
		Topology = TOPOLOGY::TRIANGLES;


	}

	//Mesh::Mesh(const unsigned int objectNTriangle, const unsigned int objectNVertex,
	//	const unsigned int* index, const float* pos, Shader * material, const float* n, const float* color, const float* tex, const float* tangent)
	//{

	//	//Indices y posiciones
	//	triangleIndex.resize(objectNTriangle * 3);
	//	vertexPos.resize(objectNVertex * 3);

	//	std::copy(index, index + objectNTriangle * 3, triangleIndex.begin());
	//	std::copy(pos, pos + objectNVertex * 3, vertexPos.begin());

	//	//Colores
	//	if (color != nullptr)
	//	{
	//		vertexColor.resize(objectNVertex * 3);
	//		std::copy(color, color + objectNVertex * 3, vertexColor.begin());
	//	}

	//	else
	//		vertexColor.clear();

	//	//Normales
	//	if (n != nullptr)
	//	{
	//		vertexNormal.resize(objectNVertex * 3);
	//		std::copy(n, n + objectNVertex * 3, vertexNormal.begin());
	//	}

	//	else
	//		vertexNormal.clear();

	//	//Coordenadas de textura
	//	if (tex != nullptr)
	//	{
	//		vertexTexCoord.resize(objectNVertex * 2);
	//		std::copy(tex, tex + objectNVertex * 2, vertexTexCoord.begin());
	//	}

	//	else
	//		vertexTexCoord.clear();

	//	//Tangentes
	//	if (tangent != nullptr)
	//	{
	//		vertexTangent.resize(objectNVertex * 3);
	//		std::copy(tangent, tangent + objectNVertex * 3, vertexTangent.begin());
	//	}

	//	else
	//		vertexTangent.clear();

	//}


	unsigned int* Mesh::getTriangleIndex()
	{
		return &triangleIndex[0];
	}

	float* Mesh::getVertexPos()
	{
		return &vertexPos[0].x;
	}

	float* Mesh::getVertexNormals()
	{
		return &vertexNormal[0].x;
	}

	float* Mesh::getVertexColor()
	{
		return &vertexColor[0].x;
	}

	float* Mesh::getVertexTexCoord()
	{
		return &vertexTexCoord[0].x;
	}

	float* Mesh::getVertexTangent()
	{
		return &vertexTangent[0].x;
	}

	int Mesh::getNumVertex()
	{
		return vertexPos.size() / 3;
	}

	int Mesh::getNumTriangles()
	{
		return triangleIndex.size() / 3;
	}

	unsigned int Mesh::getColorTex()
	{
		return colorTex;
	}

	unsigned int Mesh::getSpecTex()
	{
		return specTex;
	}

	unsigned int Mesh::getEmiTex()
	{
		return emiTex;
	}

	unsigned int Mesh::getNormTex()
	{
		return normTex;
	}

	unsigned int Mesh::getVAO()
	{
		return m_VAO;
	}



	void Mesh::Generate(bool interleaved)
	{
		if (!m_VAO)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			//TODO maybe use EBO?

		}

		std::vector<float> data;

		if (interleaved)
		{
			//TODO better if/then 
			for (int i = 0; i < vertexPos.size(); i++)
			{
				data.push_back(vertexPos[i].x);
				data.push_back(vertexPos[i].y);
				data.push_back(vertexPos[i].z);

				if (vertexTexCoord.size() > 0)
				{
					data.push_back(vertexTexCoord[i].x);
					data.push_back(vertexTexCoord[i].y);
				}
				if (vertexNormal.size() > 0)
				{
					data.push_back(vertexNormal[i].x);
					data.push_back(vertexNormal[i].y);
					data.push_back(vertexNormal[i].z);
				}
				if (vertexTangent.size() > 0)
				{
					data.push_back(vertexTangent[i].x);
					data.push_back(vertexTangent[i].y);
					data.push_back(vertexTangent[i].z);
				}
				if (vertexBitangent.size() > 0)
				{
					data.push_back(vertexBitangent[i].x);
					data.push_back(vertexBitangent[i].y);
					data.push_back(vertexBitangent[i].z);
				}

			}
		}

		else
		{
			for (int i = 0; i < vertexPos.size(); i++)
			{
				data.push_back(vertexPos[i].x);
				data.push_back(vertexPos[i].y);
				data.push_back(vertexPos[i].z);
			}
			for (int i = 0; i < vertexTexCoord.size(); i++)
			{
				data.push_back(vertexTexCoord[i].x);
				data.push_back(vertexTexCoord[i].y);
			}
			for (int i = 0; i < vertexNormal.size(); i++)
			{
				data.push_back(vertexNormal[i].x);
				data.push_back(vertexNormal[i].y);
				data.push_back(vertexNormal[i].z);
			}
			for (int i = 0; i < vertexTangent.size(); i++)
			{
				data.push_back(vertexTangent[i].x);
				data.push_back(vertexTangent[i].y);
				data.push_back(vertexTangent[i].z);
			}
			for (int i = 0; i < vertexBitangent.size(); i++)
			{
				data.push_back(vertexBitangent[i].x);
				data.push_back(vertexBitangent[i].y);
				data.push_back(vertexBitangent[i].z);
			}
		}

		// Configure Vertex atrib
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

		if (interleaved)
		{
			// Calculate stride to be applied
			size_t stride = 3 * sizeof(float);
			if (vertexTexCoord.size() > 0) stride += 2 * sizeof(float);
			if (vertexNormal.size() > 0) stride += 3 * sizeof(float);
			if (vertexTangent.size() > 0) stride += 3 * sizeof(float);
			if (vertexBitangent.size() > 0) stride += 3 * sizeof(float);

			size_t offset = 0;
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
			offset += 3 * sizeof(float);

			if (vertexTexCoord.size() > 0)
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
				offset += 2 * sizeof(float);
			}
			if (vertexNormal.size() > 0)
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}
			if (vertexTangent.size() > 0)
			{
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}
			if (vertexBitangent.size() > 0)
			{
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}

		}

		else
		{
			size_t offset = 0;
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
			offset += 3 * sizeof(float);

			if (vertexTexCoord.size() > 0)
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
				offset += 2 * sizeof(float);
			}
			if (vertexNormal.size() > 0)
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}
			if (vertexTangent.size() > 0)
			{
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}
			if (vertexBitangent.size() > 0)
			{
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
				offset += 3 * sizeof(float);
			}
		}
		glBindVertexArray(0);

	}


	Mesh::~Mesh()
	{

		// Delete VAO and VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//if (mat->getInPos() != -1) glDeleteBuffers(1, &posVBO);
		//if (mat->getInColor() != -1) glDeleteBuffers(1, &colorVBO);
		//if (mat->getInNormal() != -1) glDeleteBuffers(1, &normalVBO);
		//if (mat->getInTexCoord() != -1) glDeleteBuffers(1, &texCoordVBO);
		//if (mat->getInTangent() != -1) glDeleteBuffers(1, &tangentVBO);
		//glDeleteBuffers(1, &triangleIndexVBO);
		glDeleteBuffers(1, &m_VBO);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_VAO);

		//Delete textures
		if (colorTex != 0) glDeleteTextures(1, &colorTex);
		if (specTex != 0) glDeleteTextures(1, &specTex);
		if (emiTex != 0) glDeleteTextures(1, &emiTex);
		if (normTex != 0) glDeleteTextures(1, &normTex);
	}

}


