#include <list>

#include <windows.h>

#include <gl/glew.h> //Siempre antes que GL
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING 
#include <GLFW\glfw3.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>  //Librería matemática
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Scene.h"
#include "CatmullRom.h"


//Matrices
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f); //Una por objeto
glm::mat4	model2 = glm::mat4(1.0f); //Segundo cubo


//Malla cargada con assimp (Javi)
Mesh* malla;

//Variables para la camara
float rotation = 0.0f;
glm::vec3 translation = glm::vec3(0.0f);

float mouseSensibility = 0.001f;
bool invertMouse = false;

float pitch = 0.0f, yaw = 0.0f;
float minYawAngle = 5 * 3.14159f / 180.0f;
float maxYawAngle = 175 * 3.14159f / 180.0f;

float cameraSpeed = 0.1f;

int oldX = 0;
int oldY = 0;

bool orbital = false;

GLfloat anisotropico = -1.0;

float lightPitch = 0.0f, lightYaw = 0.0f, lightIntensity = 1.0f;

CatmullRom cr1 = CatmullRom(glm::vec3(0.0f, 0.0f,  40.0f), glm::vec3(4.0f, -5.0f, 0.0f), glm::vec3(-4.0f, 5.0f,  0.0f), glm::vec3(0.0f, 0.0f, 40.0f));
CatmullRom cr2 = CatmullRom(glm::vec3(0.0f, 0.0f, -40.0f), glm::vec3(-4.0f, 5.0f, 0.0f), glm::vec3(4.0f, -5.0f, 0.0f), glm::vec3(0.0f, 0.0f, -40.0f));

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void destroy();

//Funcion manual check extensiones
bool checkExtension(std::string extName);

GLFWwindow* taranisWindow;

Scene scene;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();


	// Motor de render: ejemplo
	Shader* shader =  scene.LoadShader("../shaders_P5/shader.v1.vert", "../shaders_P5/shader.v1.frag");
	DirectionalLight* light = scene.AddDirectionalLight();
	light->SetDirection(glm::vec3(1.0, 0.0, 1.0));

	Mesh* m = scene.LoadMesh("../meshes/statue.obj", shader);

	m->loadColorTex("../meshes/color.jpg");
	m->loadNormTex("../meshes/normales.jpg");
	m->loadSpecTex("../meshes/specular.jpg");


	Object* obj = scene.CreateObject(m, "test");
	obj->SetPosition(glm::vec3(0.0, -2.0, 0.0));
	obj->Update(0.0);

	glutMainLoop(); // Loop principal


	glfwDestroyWindow(taranisWindow);

	glfwTerminate();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv)
{
	// Crea el contexto
	if (!glfwInit())
	{
		std::cout << "Initialization failed!" << std::endl;
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

	glutInitContextProfile(GLUT_CORE_PROFILE); // No hay retrocompatibilidad

											   // Define el framebuffer y crea la ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //Elementos del buffer: doble buffer, formato RGBA y con profundidad

	// Inicializa las extensiones
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
	glfwSetWindowCloseCallback(taranisWindow, window_closed);

	glfwSetKeyCallback(taranisWindow, keyboardFunc);

	// Funciones que tratan los eventos
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST); // Activa el test de profundidad
	
	glClearColor(0.38f, 0.38f, 0.38f, 0.0f);

	glFrontFace(GL_CCW); // Counter clock wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Pintar cara front y back enteras
	glEnable(GL_CULL_FACE); // Activa el culling


	//Comprobar si se soporta el Anisotropico
	if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic") || checkExtension("GL_EXT_texture_filter_anisotropic"))
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropico);
	}

	// Matriz de perspectiva
	view = glm::mat4(1.0f);
	view[3].z = -6;
	translation.z = -6;
	scene.camera.SetViewMatrix(view);

}

void renderFunc()
{
	scene.RenderLoop();
}


void resizeFunc(int width, int height)
{

	float n = 0.1f;
	float f = 50.0f;

	scene.camera.SetPerspProjection(glm::radians(60.0f), (float)width / (float)height, n, f);

	glViewport(0, 0, width, height);

	glutPostRedisplay();
}

void idleFunc()
{
	scene.UpdateLoop();
}

void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

	glm::vec3 forwardDirection = glm::vec3(0.0f);

	forwardDirection.x = -sinf(pitch);
	forwardDirection.y = sinf(yaw);
	forwardDirection.z = cosf(pitch);

	forwardDirection = glm::normalize(forwardDirection);


	float forward = 0.0f;
	float horizontal = 0.0f;
	glm::vec3 horizontalDirection = glm::cross(forwardDirection, glm::vec3(0.0f, -1.0f, 0.0f));

	bool translate = true; //Si no se usa este booleano, los cálculos de vectores fallarán y darán errores
	if (action == GLFW_PRESS)
	{

		switch (key)
		{

		case GLFW_KEY_S:
			forward = -1.0f;
			break;
		case GLFW_KEY_W:
			forward = 1.0f;
			break;
		case GLFW_KEY_D:
			horizontal = -1.0f;
			break;
		case GLFW_KEY_A:
			horizontal = 1.0f;
			break;
		case GLFW_KEY_R:
			orbital = !orbital;
			translate = false;
			translation = glm::vec3(0.0f);
			translation.z = -7.0f;
			pitch = 0.0f;
			yaw = 0.0f;
			break;
		case GLFW_KEY_KP_4:
			lightPitch -= 0.05f;
			return;
		case GLFW_KEY_KP_6:
			lightPitch += 0.05f;
			return;
		case GLFW_KEY_KP_2:
			lightYaw += 0.05f;
			return;
		case GLFW_KEY_KP_8:
			lightYaw -= 0.05f;
			return;
		case GLFW_KEY_KP_ADD:
			lightIntensity += 0.1f;
			if (lightIntensity > 1.0f)
				lightIntensity = 1.0f;
			return;
		case GLFW_KEY_KP_SUBTRACT:
			lightIntensity -= 0.1f;
			if (lightIntensity < 0.0f)
				lightIntensity = 0.0f;
			return;
		case GLFW_KEY_ESCAPE:
			glfwWindowShouldClose(taranisWindow);
		default:
			translate = false;
			break;
		}
	}

	if (translate && !orbital)
	{
		forwardDirection *= forward;
		horizontalDirection *= horizontal;
		glm::vec3 finalDirection = forwardDirection + horizontalDirection;
		finalDirection = glm::normalize(finalDirection);
		finalDirection *= cameraSpeed;
		translation += finalDirection;
	}

	view = glm::mat4(1.0f);

	if (!orbital)
	{
		view = glm::rotate(view, yaw, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, pitch, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::translate(view, translation);
	}
	else
	{
		view = glm::translate(view, translation);
		view = glm::rotate(view, yaw, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, pitch, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	scene.camera.SetViewMatrix(view);

	glutPostRedisplay();
}
void mouseFunc(int button, int state, int x, int y)
{

	if (state == 0) {
		if (button == 0)
		{
			oldX = x;
			oldY = y;
		}
	}
	else {
		if (button == 0)
		{
			oldX = x;
			oldY = y;
		}
	}
}

void mouseMotionFunc(int x, int y)
{

	if (x != oldX)
		pitch += (float)(x - oldX) * mouseSensibility * ((invertMouse) ? -1.0f : 1.0f);

	if (y != oldY)
		yaw += (float)(y - oldY) * mouseSensibility * ((invertMouse) ? -1.0f : 1.0f);

	if (yaw > maxYawAngle)
		yaw = maxYawAngle;

	if (yaw + 3.14159f < minYawAngle)
		yaw = minYawAngle - 3.14159f;
	oldX = x; oldY = y;

	view = glm::mat4(1.0f);

	if (!orbital)
	{
		view = glm::rotate(view, yaw, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, pitch, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::translate(view, translation);
	}
	else
	{
		view = glm::translate(view, translation);
		view = glm::rotate(view, yaw, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, pitch, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	scene.camera.SetViewMatrix(view);
	
	glutPostRedisplay();
}

bool checkExtension(std::string extName)
{
	int NumExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumExtensions);
	
	const GLubyte *test = (GLubyte *)(extName.c_str());

	//Para cada extension
	for (int i = 0; i < NumExtensions; i++)
	{
		const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);


		if (strcmp((const char *)ccc, (const char *)test) == 0)
		{
			return true;
		}
	}
	return false;
}

void window_closed(GLFWwindow *window)
{
	std::cout << "Bye!" << std::endl;

	glfwDestroyWindow(window);

	glfwTerminate();

	system("pause");
	exit(0);
}
