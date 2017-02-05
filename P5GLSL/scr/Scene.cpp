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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia buffer de color y profundidad

	sceneShaders.at(0).render(sceneLights, camera);

	glUseProgram(NULL);
	
	glutSwapBuffers(); // Swap de los buffers
	//TODO implementar multiples pasadas

	//for (Shader &shader : sceneShaders)
	//{
	//	//Pasar vector luces y matrices vista y proyección
	//}
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

	return &sceneShaders.back();
}

//Loads mesh, adds it to the mesh vectr and returns it
Mesh* Scene::LoadMesh(char* mesh, Shader* shader)
{
	Mesh newMesh(mesh);

	sceneMeshes.push_back(newMesh);

	Mesh *ret = &sceneMeshes.back();

	shader->addMesh(ret);

	return ret;
}

//Loads mesh, adds it to the mesh vectr and returns it
Mesh* Scene::LoadMesh(Mesh* mesh, Shader* shader)
{
	sceneMeshes.push_back(*mesh);

	Mesh *ret = &sceneMeshes.back();

	shader->addMesh(ret);

	return ret;
}

//Creates object, adds it to the object vector, and returns it
Object* Scene::CreateObject(Mesh* mesh, char* name)
{
	Object newObject;
	newObject.InitObject(name, nullptr, mesh);

	sceneObjects.push_back(newObject);

	Object *ret = &sceneObjects.back();

	mesh->addObject(ret);

	return ret;
}

//Adds light to the scene and returns it
DirectionalLight* Scene::AddDirectionalLight()
{
	DirectionalLight newLight;

	sceneLights.push_back(newLight);

	return static_cast<DirectionalLight*>(&sceneLights.back());
}

PointLight* Scene::AddPointLight()
{
	PointLight newLight;

	sceneLights.push_back(newLight);

	return  static_cast<PointLight*>(&sceneLights.back());
}

SpotLight* Scene::AddSpotLight()
{
	SpotLight newLight;

	sceneLights.push_back(newLight);


	return static_cast<SpotLight*>(&sceneLights.back());
}
