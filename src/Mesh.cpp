#include "Mesh.h"

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>         
//#include <assimp/postprocess.h>

#include <gl/glew.h> //Siempre antes que GL
#include <gl/gl.h>
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
#include "Shader.h"

#define MAX 100

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


void Mesh::Loader(std::string fileName, bool generateNormals=false, bool generateTangets=false)
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

			triangleIndex.push_back(v1);
			triangleIndex.push_back(v2);
			triangleIndex.push_back(v3);


			//Quad --> split second triangle
			if (v4 != -1)
			{
				triangleIndex.push_back(v4);
				triangleIndex.push_back(v1);
				triangleIndex.push_back(v3);
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

			vertexNormal[i] += p;
			vertexNormal[i + 1] += p;
			vertexNormal[i + 2] += p;

		}

		for (int i = 0; i < vertexNormal.size(); i++)
			vertexNormal[i] = glm::normalize(vertexNormal[i]);

	}

	//Tangents
	if (generateTangets && vertexTangent.size() == 0)
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







//TODO mejorar, solo genera la primera malla del archivo
Mesh::Mesh(const char* file, Shader * material)
{

	Loader(std::string(file), true, true);

	ApplyMaterial(material);


}

Mesh::Mesh(const unsigned int objectNTriangle, const unsigned int objectNVertex,
	const unsigned int* index, const float* pos, Shader * material, const float* n, const float* color, const float* tex, const float* tangent)
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

	ApplyMaterial(material);
}

void Mesh::ApplyMaterial(Shader * material)
{
	mat = material;
	generateVAO();
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


 unsigned char*  Mesh::loadTexture(const char* fileName, unsigned int &w, unsigned int &h)
 {
	 FreeImage_Initialise(TRUE);

	 FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	 if (format == FIF_UNKNOWN)
		 format = FreeImage_GetFIFFromFilename(fileName);
	 if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
		 return NULL;

	 FIBITMAP* img = FreeImage_Load(format, fileName);
	 if (img == NULL)
		 return NULL;

	 FIBITMAP* tempImg = img;
	 img = FreeImage_ConvertTo32Bits(img);
	 FreeImage_Unload(tempImg);

	 w = FreeImage_GetWidth(img);
	 h = FreeImage_GetHeight(img);

	 //BGRA a RGBA
	 unsigned char * map = new unsigned char[4 * w*h];
	 char *buff = (char*)FreeImage_GetBits(img);

	 for (unsigned int j = 0; j<w*h; j++){
		 map[j * 4 + 0] = buff[j * 4 + 2];
		 map[j * 4 + 1] = buff[j * 4 + 1];
		 map[j * 4 + 2] = buff[j * 4 + 0];
		 map[j * 4 + 3] = buff[j * 4 + 3];
	 }

	 FreeImage_Unload(img);
	 FreeImage_DeInitialise();

	 return map;
 }

 unsigned int Mesh::loadTex(const char* fileName)
 {
	 //Carga textura de fichero
	 unsigned char *map;
	 unsigned int w, h;
	 map = loadTexture(fileName, w, h);
	 if (!map) //Si devuelve null es que no se ha cargado la textura
	 {
		 return -1;
	 }

	 unsigned int texId = 0;
	 glGenTextures(1, &texId);
	 glBindTexture(GL_TEXTURE_2D, texId);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		 GL_UNSIGNED_BYTE, (GLvoid*)map); //Reserva espacio y sube textura, primer 0 indica los niveles de detalle, RGBA8: formato interno de la tarjeta gráfica (como se almacenan los datos), RGBA: formato en el que se pasan los datos, Unsigned: como son los datos

	 delete[] map; //Borrar de memoria la textura original

	 glGenerateMipmap(GL_TEXTURE_2D); //Genera midmaps

	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		 GL_LINEAR_MIPMAP_LINEAR); //Filtrado trilinear
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Filtrado lineal
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	 ////Anisotropico si es posible
	 //if (anisotropico != -1.0f)
	 // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropico);

	 return texId; //Devuelve el identificador de textura
 }

 void Mesh::generateVAO()
 {
	 glGenVertexArrays(1, &m_VAO);
	 glBindVertexArray(m_VAO);

	 if (mat->getInPos() != -1 && vertexPos.size() > 0) //Localizador de la posición con la información de las posiciones de los vértices
	 {
		 glGenBuffers(1, &posVBO); //NO TOCA VAO
		 glBindBuffer(GL_ARRAY_BUFFER, posVBO); //Activa VBO
		 glBufferData(GL_ARRAY_BUFFER, getNumVertex() * sizeof(float) * 3,
			 getVertexPos(), GL_STATIC_DRAW); //Sube información
		 glVertexAttribPointer(mat->getInPos(), 3, GL_FLOAT, GL_FALSE, 0, 0); //Vertice tiene tres elementos, del tipo float, no notmaliza, stride y offset
		 glEnableVertexAttribArray(mat->getInPos()); //Configura en el VAO que tiene que utilizar inPos. Si no se quiere utilizar, llamar a glDisable...
	 }
	 if (mat->getInColor() != -1 && vertexColor.size() > 0) //Se repite como en inPos
	 {
		 glGenBuffers(1, &colorVBO);
		 glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		 glBufferData(GL_ARRAY_BUFFER, getNumVertex() * sizeof(float) * 3,
			 getVertexColor(), GL_STATIC_DRAW);
		 glVertexAttribPointer(mat->getInColor(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(mat->getInColor());
	 }
	 if (mat->getInNormal() != -1 && vertexNormal.size() > 0)
	 {
		 glGenBuffers(1, &normalVBO);
		 glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		 glBufferData(GL_ARRAY_BUFFER, getNumVertex() * sizeof(float) * 3,
			getVertexNormals(), GL_STATIC_DRAW);
		 glVertexAttribPointer(mat->getInNormal(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(mat->getInNormal());
	 }
	 if (mat->getInTexCoord() != -1 && vertexTexCoord.size() > 0)
	 {
		 glGenBuffers(1, &texCoordVBO);
		 glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		 glBufferData(GL_ARRAY_BUFFER, getNumVertex() * sizeof(float) * 2,
			 getVertexTexCoord(), GL_STATIC_DRAW);
		 glVertexAttribPointer(mat->getInTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(mat->getInTexCoord());
	 }
	 if (mat->getInTangent() != -1 && vertexTangent.size() > 0)
	 {
		 glGenBuffers(1, &tangentVBO);
		 glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		 glBufferData(GL_ARRAY_BUFFER, getNumVertex() * sizeof(float) * 3,
			 getVertexTangent(), GL_STATIC_DRAW);
		 glVertexAttribPointer(mat->getInTangent(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(mat->getInTangent());
	 }

	 //Indica como recorrer los elementos
	 glGenBuffers(1, &triangleIndexVBO);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 this->getNumTriangles() * sizeof(unsigned int) * 3, this->getTriangleIndex(),
		 GL_STATIC_DRAW);
 }

 bool Mesh::loadColorTex(const char* fileName)
 {
	 colorTex = loadTex(fileName);
	 return colorTex != 0;
 }

 bool Mesh::loadSpecTex(const char* fileName)
 {
	 specTex = loadTex(fileName);
	 return specTex != 0;
 }

 bool  Mesh::loadEmiTex(const char* fileName)
 {
	 emiTex = loadTex(fileName);
	 return emiTex != 0;
 }
 bool Mesh::loadNormTex(const char* fileName)
 {
	 normTex = loadTex(fileName);
	 return normTex != 0;
 }

 void Mesh::addObject(Object* o)
 {
	 objects.push_back(o);
 }

 void Mesh::removeObject(Object* o)
 {
	 objects.remove(o);
 }

 //void Mesh::render(Camera &camera, Shader &shader)
 //{
	// for (Object* o : objects)
	// {
	//	 //Subir las variables de este objeto

	//	 //Se suben las variables uniformes
	//	 glm::mat4 modelView = *camera.GetView() * *o->GetModelMatrix();
	//	 glm::mat4 modelViewProj = *camera.GetProjection() * *camera.GetView() * *o->GetModelMatrix();
	//	 glm::mat4 normal = glm::transpose(glm::inverse(modelView));


	//	 if (uModelViewMat != -1) // Identificador a la variable uniforme
	//		 glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
	//			 &(modelView[0][0]));
	//	 if (uModelViewProjMat != -1)
	//		 glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
	//			 &(modelViewProj[0][0]));
	//	 if (uNormalMat != -1)
	//		 glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
	//			 &(normal[0][0]));

	//	 //Texturas
	//	 if (uColorTex != -1)
	//	 {
	//		 glActiveTexture(GL_TEXTURE0); //Activa texture unit 0
	//		 glBindTexture(GL_TEXTURE_2D, colorTexId);
	//		 glUniform1i(uColorTex, 0);
	//	 }
	//	 if (uEmiTex != -1)
	//	 {
	//		 glActiveTexture(GL_TEXTURE0 + 1); //Activa texture unit 1
	//		 glBindTexture(GL_TEXTURE_2D, emiTexId);
	//		 glUniform1i(uEmiTex, 1);
	//	 }
	//	 if (uSpecTex != -1)
	//	 {
	//		 glActiveTexture(GL_TEXTURE0 + 2); //Activa texture unit 2 (especular)
	//		 glBindTexture(GL_TEXTURE_2D, specTexId);
	//		 glUniform1i(uSpecTex, 2);
	//	 }
	//	 if (uNormTex != -1)
	//	 {
	//		 glActiveTexture(GL_TEXTURE0 + 3); //Activa texture unit 3 (normal)
	//		 glBindTexture(GL_TEXTURE_2D, normTexId);
	//		 glUniform1i(uNormTex, 3);
	//	 }
	// }
 //}

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

std::list<Object*>& Mesh::getObjects()
{
	return objects;
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
