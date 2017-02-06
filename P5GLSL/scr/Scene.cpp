#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	//camera.SetPerspProjection(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);

}


Scene::~Scene()
{

}

void Scene::RenderLoop()
{
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
	QueryPerformanceFrequency(&m_frequency);

	// start timer
	QueryPerformanceCounter(&m_t1);

	// do something
	for (Object &object : sceneObjects)
	{
		object.Update(m_elapsedTime);
	}

	// stop timer
	QueryPerformanceCounter(&m_t2);

	// compute and print the elapsed time in seconds
	m_elapsedTime = (m_t2.QuadPart - m_t1.QuadPart) * 1000.0 / m_frequency.QuadPart;
}

//Loads and compiles shader, adds it to the shader vector and returns it
Shader* Scene::LoadShader(char* vertexShader, char* fragmentShader)
{
	sceneShaders.emplace_back(vertexShader, fragmentShader);

	return &sceneShaders.back();
}

//Loads mesh, adds it to the mesh vectr and returns it
Mesh* Scene::LoadMesh(char* mesh, Shader* shader)
{
	
	sceneMeshes.emplace_back(mesh, shader);

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

	sceneObjects.emplace_back();

	Object *ret = &sceneObjects.back();
	ret->InitObject(name, nullptr, mesh);

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

	return static_cast<PointLight*>(&sceneLights.back());
}

SpotLight* Scene::AddSpotLight()
{
	SpotLight newLight;

	sceneLights.push_back(newLight);


	return static_cast<SpotLight*>(&sceneLights.back());
}
