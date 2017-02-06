#pragma once

#include <vector>
#include <list>
#include <time.h>

#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Camera.h"

#include "Light.h""
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

//!  Camera class. 
/*!
Class used to set a scene to render. Contains essential functions to add lights, meshes, objects and set render loops.
*/
class Scene
{
public:

	//! Class constructor.
	/*!
	Scene class constructor.
	*/
	Scene();

	//! Class descturctor.
	/*!
	Scene class destructor.
	*/
	~Scene();

	//! Camera used to render the Scene.
	Camera camera;

	//! Vector of shaders used in this Scene.
	std::vector<Shader> sceneShaders;

	//! Vector of meshes used in this Scene.
	std::vector<Mesh> sceneMeshes;

	//! Vector of objects used in this Scene.
	std::vector<Object> sceneObjects;

	//! Vector of lights used in this Scene.
	std::vector<Light> sceneLights;

	//! Function to call in the render loop of the program.
	/*!
	Render all the objects in the scene using the requiered shaders and meshes.
	*/
	void RenderLoop();

	//! Function to call in the update loop of the program.
	/*!
	Work in Progress.
	*/
	void UpdateLoop();

	//Loads and compiles shader, adds it to the shader vector and returns it
	//! Function to add a Shader to this Scene.
	/*!
	\param vertexShader Path to vertex shader file.
	\param vertexShader Path to fragment shader file.
	\return The compiled shader added to the Scene
	*/
	Shader* LoadShader(char* vertexShader, char* fragmentShader);

	//! Function to add a Mesh to this Scene.
	/*!

	Creates a mesh from a file and adds it to this Scene using shader.

	\param mesh Path to mesh file.
	\param shader Shader to use to render this mesh.
	\return The created Mesh added to the Scene.
	*/
	Mesh* LoadMesh(char* mesh, Shader* shader);

	//! Function to add a Mesh to this Scene.
	/*!

	Adds a Mesh to this Scene using shader.

	\param mesh Mesh to add.
	\param shader Shader to use to render this mesh.
	\return The Mesh added to the Scene.
	*/
	Mesh* LoadMesh(Mesh* mesh, Shader* shader);

	//! Function to add an Object to this Scene.
	/*!

	Creates a Object using a Mesh and adds it to this Scene.

	\param mesh Mesh to use in the Object.
	\param name Debug name of this Object.
	\return The created Object added to the Scene.
	*/
	Object* CreateObject(Mesh* mesh, char* name);
	
	//! Function to add a Directional Light to this Scene.
	/*!

	Creates a Directional Light and adds it to the Scene.

	\return The created Light added to the Scene.
	*/
	DirectionalLight* AddDirectionalLight();

	//! Function to add a Point Light to this Scene.
	/*!

	Creates a Point Light and adds it to the Scene.

	\return The created Light added to the Scene.
	*/
	PointLight* AddPointLight();

	//! Function to add a Spot Light to this Scene.
	/*!

	Creates a Spot Light and adds it to the Scene.

	\return The created Light added to the Scene.
	*/
	SpotLight* AddSpotLight();

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_t1, m_t2;
	double m_elapsedTime;


};

