#pragma once

#include <vector>
#include <list>

#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Camera.h"

#include "Light.h""
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Scene
{
public:
	Scene();
	~Scene();

	//¿Considerar la cámara como un objeto?
	Camera camera;

	std::vector<Shader> sceneShaders;
	std::vector<Mesh> sceneMeshes;
	std::vector<Object> sceneObjects;
	std::list<Light> sceneLights; //std::vector<Light> sceneLights;

	float deltaTime;

	void RenderLoop();
	void UpdateLoop();

	//Loads and compiles shader, adds it to the shader vector and returns it
	Shader* LoadShader(char* vertexShader, char* fragmentShader);

	//Loads mesh, adds it to the mesh vectr and returns it
	Mesh* LoadMesh(char* mesh, Shader* shader);
	Mesh* LoadMesh(Mesh* mesh, Shader* shader);

	//Creates object, adds it to the object vector, and returns it
	Object* CreateObject(Mesh* mesh, char* name);
	
	//Adds light to the scene and returns it
	DirectionalLight* AddDirectionalLight();
	PointLight* AddPointLight();
	SpotLight* AddSpotLight();

	//Deleters


};

