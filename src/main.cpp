

#include <gl/glew.h> //Siempre antes que GL
//#include <gl/gl.h>
#include <GLFW\glfw3.h>

#include "renderer\renderer.h"

#include "utils\log.h"
#include "mesh\mesh.h"

#include <iostream>

#include "mesh/mesh.h"
#include "renderer/renderer.h"




GLFWwindow* taranisWindow; // Window



void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void InitContext()
{

	if (!glfwInit())
	{
		std::cout << "GLFW initalization failed!" << std::endl;
		system("pause");
		exit(-1);
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	taranisWindow = glfwCreateWindow(500, 500, "Taranis Engine", NULL, NULL);
	if (!taranisWindow)
	{
		std::cout << "Couldn't create window!" << std::endl;
		system("pause");
		exit(-1);
	}

	glfwMakeContextCurrent(taranisWindow);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) //Pregunta si ha habido algún error
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	//Callback para cuando se cierra la ventana
	//glfwSetWindowCloseCallback(taranisWindow, window_closed);

	//// Funciones que tratan los eventos
	//glfwSetKeyCallback(taranisWindow, keyboardFunc);
	//glfwSetWindowSizeCallback(taranisWindow, resizeFunc);
	//glfwSetCursorPosCallback(taranisWindow, mouseMotionFunc);
	//glfwSetMouseButtonCallback(taranisWindow, mouseFunc);

	//Evitar tearing
	glfwSwapInterval(1);

}

void initOGL()
{
	glEnable(GL_DEPTH_TEST); // Activa el test de profundidad

	glClearColor(0.38f, 0.38f, 0.38f, 0.0f);

	glFrontFace(GL_CCW); // Counter clock wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Pintar cara front y back enteras
	glEnable(GL_CULL_FACE); // Activa el culling


	//						//Comprobar si se soporta el Anisotropico
	//if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic") || checkExtension("GL_EXT_texture_filter_anisotropic"))
	//{
	//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropico);
	//}

	//// Matriz de perspectiva
	//view = glm::mat4(1.0f);
	//view[3].z = -6;
	//translation.z = -6;
	//scene.camera.SetViewMatrix(view);

}

void main()
{
	InitContext();
	initOGL();

	// Example mesh
	Taranis::Mesh* mesh = new Taranis::Mesh("D:/Proyectos/Taranis/bunny.obj");
	mesh->Generate();

	Camera* myCamera = new Camera();

	Taranis::Renderer myRenderer;
	myRenderer.Init();
	myRenderer.SetCamera(myCamera);
	myRenderer.SetRenderSize(500, 500);

	while (!glfwWindowShouldClose(taranisWindow))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myRenderer.RenderPushedCommands();

		glfwSwapBuffers(taranisWindow);

	}


	system("pause");
	
}