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

//Carga de texturas
//#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#include "Shader.h"

void Mesh::Loader(std::string fileName)
{


	std::ifstream is(fileName);
	if (is.fail()) throw "Unable to open " + fileName;

	std::cout << "Loading " << fileName << std::endl;
	std::cout.flush();

	std::string lineString;
	while (std::getline(is, lineString))
	{
		std::istringstream line(lineString);

		std::string prefix; //Line prefix
		line >> prefix;



		if (prefix == "v") //Vertex
		{
			glm::vec3 p;
			line >> p.x >> p.y >> p.z;
			
			vertexPos.push_back(p.x);
			vertexPos.push_back(p.y);
			vertexPos.push_back(p.z);
			
		}

		else if (prefix == "vt") //Tex Coord
		{
			glm::vec2 tc;
			line >> tc.x >> tc.y;

			vertexTexCoord.push_back(tc.x);
			vertexTexCoord.push_back(tc.y);
		}

		else if (prefix == "vn") //Normal
		{
			glm::vec3 n;
			line >> n.x >> n.y >> n.z;

			vertexNormal.push_back(n.x);
			vertexNormal.push_back(n.y);
			vertexNormal.push_back(n.z);

		}

		else if (prefix == "f") //Face
		{
			//Update

			unsigned int v1, v2, v3, v4=-1;
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
	}

}







//TODO mejorar, solo genera la primera malla del archivo
Mesh::Mesh(const char* file, Shader * material)
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
	 glGenVertexArrays(1, &vao);
	 glBindVertexArray(vao);

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
	 return vao;
}

std::list<Object*>& Mesh::getObjects()
{
	return objects;
}


Mesh::~Mesh()
{

	// Delete VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (mat->getInPos() != -1) glDeleteBuffers(1, &posVBO);
	if (mat->getInColor() != -1) glDeleteBuffers(1, &colorVBO);
	if (mat->getInNormal() != -1) glDeleteBuffers(1, &normalVBO);
	if (mat->getInTexCoord() != -1) glDeleteBuffers(1, &texCoordVBO);
	if (mat->getInTangent() != -1) glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	//Delete textures
	if (colorTex != 0) glDeleteTextures(1, &colorTex);
	if (specTex != 0) glDeleteTextures(1, &specTex);
	if (emiTex != 0) glDeleteTextures(1, &emiTex);
	if (normTex != 0) glDeleteTextures(1, &normTex);
}
