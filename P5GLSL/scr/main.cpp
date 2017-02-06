#include "BOX.h"
#include "auxiliar.h"
#include <list>

#include <windows.h>

#include <gl/glew.h> //Siempre antes que GL
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>  //Librería matemática
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Scene.h"
//#include "Mesh.h"
#include "CatmullRom.h"


#define CUBE 0 //Si es 1 se pinta cubo, si es 0 se carga un modelo

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f); //Una por objeto
glm::mat4	model2 = glm::mat4(1.0f); //Segundo cubo


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;


//Luces
struct LightParams {
	//Identificadores
	int uAmb;
	int uDiff;
	int uPos;
	int uDir;
	int uC;
	int uCosCutOff;
	int uSpotExponent;

	//Valores
	glm::vec3 Amb; // Intesidad ambiental
	glm::vec3 Diff; // Intensidad difusa
	glm::vec4 Pos; // Posición
	glm::vec4 Dir; // Dirección
	glm::vec3 C; // Atenuación
	GLfloat CosCutOff;
	GLfloat SpotExponent;
};
#define MAX_LIGHTS 2
LightParams lights[MAX_LIGHTS];

//Textures
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

//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int tangentVBO;
unsigned int triangleIndexVBO;
//Texturas
unsigned int colorTexId;
unsigned int emiTexId;
unsigned int normTexId;

//Especular (añadida por Borja)
unsigned int specTexId;

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

Scene scene;

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();

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
	//destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv)
{
	// Crea el contexto
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE); // No hay retrocompatibilidad

											   // Define el framebuffer y crea la ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //Elementos del buffer: doble buffer, formato RGBA y con profundidad
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas GLSL");

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
void destroy()
{
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);
	glDeleteTextures(1, &specTexId);
	glDeleteTextures(1, &normTexId);


	delete malla;
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
	model = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, -2.0, 0.0));

	//Segundo cubo
	model2 = glm::mat4(1.0f);
	static float angle2 = 0.0f;
	static float t = 0.0f;
	static bool useCr1 = true;
	angle2 = (angle2 > 3.141592f * 2.0f) ? 0 : angle2 + 0.01f;

	//t = (t > 1.0f) ? 0.0f : t + 0.005f; 
	t += 0.005f;
	if (t > 1.0f)
	{
		useCr1 = !useCr1;
		t = 0.0f;
	}

	model2 = glm::translate(model2, ((useCr1) ? cr1 : cr2).interpolate(t));//glm::vec3(2.0f, 0.0f, 0.0f));
	model2 = glm::rotate(model2, angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	//model2 = glm::translate(model2, cubicSpline.interpolate(t));//glm::vec3(2.0f, 0.0f, 0.0f));
	//model2 = glm::translate(model2, glm::vec3(0.0, -2.0, 0.0));

	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
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
	switch (key)
	{

	case 's':
		forward = -1.0f;
		break;

	case 'w':
		forward = 1.0f;
		break;

	case 'd':
		horizontal = -1.0f;
		break;

	case 'a':
		horizontal = 1.0f;
		break;

	case 'r':
		orbital = !orbital;
		translate = false;
		translation = glm::vec3(0.0f);
		translation.z = -7.0f;
		pitch = 0.0f;
		yaw = 0.0f;
		break;
	case '4':
		lightPitch -= 0.05f;
		return;
	case '6':
		lightPitch += 0.05f;
		return;
	case '2':
		lightYaw += 0.05f;
		return;
	case '8':
		lightYaw -= 0.05f;
		return;
	case '+':
		lightIntensity += 0.1f;
		if (lightIntensity > 1.0f)
			lightIntensity = 1.0f;
		return;
	case '-':
		lightIntensity -= 0.1f;
		if (lightIntensity < 0.0f)
			lightIntensity = 0.0f;
		return;

	default:
		translate = false;
		break;
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

}
void mouseFunc(int button, int state, int x, int y)
{

	if (state == 0) {
#if DEBUG_TEXT
		std::cout << "Se ha pulsado el botón ";
#endif
		if (button == 0)
		{
			oldX = x;
			oldY = y;
		}
	}

	else {
#if DEBUG_TEXT
		std::cout << "Se ha soltado el botón ";
#endif

		if (button == 0)
		{
			oldX = x;
			oldY = y;
		}

	}

#if DEBUG_TEXT
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
#endif
}

void mouseMotionFunc(int x, int y)
{

#if DEBUG_TEXT
	std::cout << "Moviendo hacia ";

	if (oldX < x) {

		std::cout << "derecha";

	}
	else if (oldX > x)
	{
		std::cout << "izquierda";
	}
#endif

	if (x != oldX)
		pitch += (float)(x - oldX) * mouseSensibility * ((invertMouse) ? -1.0f : 1.0f);

#if DEBUG_TEXT
	std::cout << "  -  ";

	if (oldY < y) std::cout << "abajo";
	else if (oldY > y) std::cout << "arriba";

#endif

	if (y != oldY)
		yaw += (float)(y - oldY) * mouseSensibility * ((invertMouse) ? -1.0f : 1.0f);

	if (yaw > maxYawAngle)
		yaw = maxYawAngle;

	if (yaw + 3.14159f < minYawAngle)
		yaw = minYawAngle - 3.14159f;
	oldX = x; oldY = y;

#if DEBUG_TEXT
	std::cout << "Pitch: " << pitch << " Yaw: " << yaw + 3.14159f << std::endl;

	std::cout << std::endl;
#endif


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







