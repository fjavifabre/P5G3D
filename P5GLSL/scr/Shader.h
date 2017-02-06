#pragma once

#include "Mesh.h"
#include <list>

#include <gl/glew.h> //Siempre antes que GL
#include <gl/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>  //Librería matemática
#include <glm/gtc/matrix_transform.hpp>
#include "Light.h"
#include "Camera.h"

#define MAX_LIGHTS 2

class Shader
{

//Identificadores de luces
struct LightIds {
		//Identificadores
		int uAmb;
		int uDiff;
		int uPos;
		int uDir;
		int uC;
		int uCosCutOff;
		int uSpotExponent;


};
	
private:
	
	//Lista de mallas(e internamente objetos) usados en este shader
	std::list<Mesh*> meshes;

	//identificadores de programa y shaders
	unsigned int vshader;
	unsigned int fshader;
	unsigned int program;

	//Variables Uniform
	int uModelViewMat;
	int uModelViewProjMat;
	int uNormalMat;
	LightIds lights[MAX_LIGHTS];

	//Texturas Uniform
	int uColorTex;
	int uEmiTex;
	int uSpecTex;
	int uNormTex;

	//Atributos
	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;
	int inTangent;


	//shader loader helper
	static GLuint loadShader(const char *fileName, GLenum type);
	static char* loadStringFromFile(const char *fileName, unsigned int &fileLen);

public:
	Shader(const char* vertex, const char* fragment);

	//Renderizar usando las luces definidas en escena
	void render(std::list<Light> &lightV, Camera &camera); //TODO: chage to camera obj

	void addMesh(Mesh *mesh);

	int getInPos();
	int getInColor();
	int getInNormal();
	int getInTexCoord();
	int getInTangent();

	~Shader();


};

