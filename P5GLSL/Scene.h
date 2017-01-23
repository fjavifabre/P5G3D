#pragma once

#include <vector>

#include "Shader.h"
#include "Mesh.h"
#include "Object.h"

#include "Light.h""
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Shader> sceneShaders;
	std::vector<Mesh> sceneMeshes;
	std::vector<Object> sceneObjects;
	std::vector<Light> sceneLights;

	//Loads and compiles shader, adds it to the shader vector and returns it
	Shader* LoadShader(char* vertexShader, char* fragmentShader);

	//Loads mesh, adds it to the mesh vectr and returns it
	Mesh* LoadMesh(char* mesh, char* name);

	//Creates object, adds it to the object vector, and returns it
	Object* CreateObject(Mesh* mesh, char* name);

	//Adds light to the scene and returns it
	Light* AddLight();
	DirectionalLight* AddDirectionalLight();
	PointLight* AddPointLight();
	SpotLight* AddSpotLight();
};

