#include "Scene.h"


Scene::Scene()
{
	//camera.SetPerspProjection(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);

}


Scene::~Scene()
{
}

void Scene::RenderLoop()
{
	//glm::mat4* proj = camera.GetProjection();
	//glm::mat4* view = camera.GetView();

	for (Shader &shader : sceneShaders)
	{
		//Pasar vector luces y matrices vista y proyección
	}
}
void Scene::UpdateLoop()
{
	for (Object &object : sceneObjects)
	{
		object.Update(deltaTime);
	}
}

//Loads and compiles shader, adds it to the shader vector and returns it
Shader* Scene::LoadShader(char* vertexShader, char* fragmentShader)
{
	Shader newShader(vertexShader, fragmentShader);

	sceneShaders.push_back(newShader);

	return &newShader;
}

//Loads mesh, adds it to the mesh vectr and returns it
Mesh* Scene::LoadMesh(char* mesh, char* name)
{
	Mesh newMesh(mesh);

	sceneMeshes.push_back(newMesh);

	return &newMesh;
}

//Creates object, adds it to the object vector, and returns it
Object* Scene::CreateObject(Mesh* mesh, char* name)
{
	Object newObject;
	newObject.InitObject(name, nullptr, mesh);

	sceneObjects.push_back(newObject);

	mesh->addObject(&newObject);

	return &newObject;
}

//Adds light to the scene and returns it
DirectionalLight* Scene::AddDirectionalLight()
{
	DirectionalLight newLight;

	sceneLights.push_back(newLight);

	return &newLight;
}

PointLight* Scene::AddPointLight()
{
	PointLight newLight;

	sceneLights.push_back(newLight);

	return &newLight;
}

SpotLight* Scene::AddSpotLight()
{
	SpotLight newLight;

	sceneLights.push_back(newLight);


	return &newLight;
}
