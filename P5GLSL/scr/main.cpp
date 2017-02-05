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
glm::mat4	proj = glm::mat4(1.0f);
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

//BSpline cubicSpline2 = BSpline(glm::vec3(-2.0f, .0f, -2.0f), glm::vec3(-2.0f, .0f, -2.0f), glm::vec3(2.0f, .0f, -2.0f), glm::vec3(-2.0f, .0f, -2.0f));

//BSpline cubicSpline1= BSpline(glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(1.0f, 2.0f, -1.0f), glm::vec3(-1.0f, -2.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
////BSpline cubicSpline2 = BSpline(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-1.0f, 2.0f, -1.0f), glm::vec3(1.0f, -2.0f, 1.0f), glm::vec3(-2.0f, 0.0f, -2.0f));
//BSpline cubicSpline2 = BSpline(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-1.0f, 2.0f, -1.0f), glm::vec3(1.0f, -2.0f, 1.0f), glm::vec3(-2.0f, 0.0f, -2.0f));

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
void initObj();
void initLights(); // Crea las luces
void destroy();

//Funcion manual check extensiones
bool checkExtension(std::string extName);


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);


Scene scene;

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();

	Shader* shader =  scene.LoadShader("../shaders_P5/shader.v1.vert", "../shaders_P5/shader.v1.frag");
	scene.AddPointLight();

	Mesh* m = scene.LoadMesh("../meshes/statue.obj",shader);
	m->loadColorTex("../meshes/color.jpg");
	m->loadNormTex("../meshes/normales.jpg");
	m->loadSpecTex("../meshes/specular.jpg");


	Object* obj = scene.CreateObject(m, "test");
	obj->SetPosition(glm::vec3(0.0, -6.0, 0.0));
	


	//initShader("../shaders_P5/shader.v1.vert", "../shaders_P5/shader.v1.frag");
	//initObj();
	//initLights(); // Inicializa las luces
	glutMainLoop(); // Loop principal
	//destroy();

	return 0;
}

//int main(int argc, char** argv)
//{
//	std::locale::global(std::locale("spanish"));// acentos ;)
//
//	initContext(argc, argv);
//	initOGL();
//	initShader("../shaders_P5/shader.v1.vert", "../shaders_P5/shader.v1.frag");
//	initObj();
//	initLights(); // Inicializa las luces
//	glutMainLoop(); // Loop principal
//	destroy();
//
//	return 0;
//}

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
							 //glClearColor(1.0f, 0.2f, 0.2f, 0.0f);
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
	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -6;
	translation.z = -6;

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

void initShader(const char *vname, const char *fname)
{
	// Compila los shaders
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	// Crear programa
	program = glCreateProgram();
	// Asignar los shaders
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	// IMPORTANTE Los identificadores de los atributos ANTES DEL LINKEADO
	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");

	// Linkearlos
	glLinkProgram(program);

	//Comprobar error
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		exit(-1);
	}

	// DESPUÉS DEL LINKEADO
	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	//Identificadores de textura
	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecTex = glGetUniformLocation(program, "specTex");
	uNormTex = glGetUniformLocation(program, "normalTex");

	//Array de luces
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		std::string number = std::to_string(i);

		lights[i].uAmb = glGetUniformLocation(program, ("lights[" + number + "].Amb").c_str());

		lights[i].uDiff = glGetUniformLocation(program, ("lights[" + number + "].Diff").c_str());

		lights[i].uPos = glGetUniformLocation(program, ("lights[" + number + "].Pos").c_str());

		lights[i].uDir = glGetUniformLocation(program, ("lights[" + number + "].Dir").c_str());

		lights[i].uC = glGetUniformLocation(program, ("lights[" + number + "].C").c_str());

		lights[i].uCosCutOff = glGetUniformLocation(program, ("lights[" + number + "].CosCutOff").c_str());

		lights[i].uSpotExponent = glGetUniformLocation(program, ("lights[" + number + "].SpotExponent").c_str());

	}


	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
	inTangent = glGetAttribLocation(program, "inTangent");

}
void initObj()
{

#if CUBE
	malla = new Mesh(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, cubeVertexPos, cubeVertexNormal, cubeVertexColor, cubeVertexTexCoord, cubeVertexTangent);
#else
	malla = new Mesh("../meshes/statue.obj");
#endif

	//Activar el Vertex Attribute 


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (inPos != -1) //Localizador de la posición con la información de las posiciones de los vértices
	{
		glGenBuffers(1, &posVBO); //NO TOCA VAO
		glBindBuffer(GL_ARRAY_BUFFER, posVBO); //Activa VBO
		glBufferData(GL_ARRAY_BUFFER, malla->getNumVertex() * sizeof(float) * 3,
			malla->getVertexPos(), GL_STATIC_DRAW); //Sube información
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0); //Vertice tiene tres elementos, del tipo float, no notmaliza, stride y offset
		glEnableVertexAttribArray(inPos); //Configura en el VAO que tiene que utilizar inPos. Si no se quiere utilizar, llamar a glDisable...
	}
	if (inColor != -1) //Se repite como en inPos
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, malla->getNumVertex() * sizeof(float) * 3,
			malla->getVertexColor(), GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, malla->getNumVertex() * sizeof(float) * 3,
			malla->getVertexNormals(), GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, malla->getNumVertex() * sizeof(float) * 2,
			malla->getVertexTexCoord(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1)
	{
		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, malla->getNumVertex() * sizeof(float) * 3,
			malla->getVertexTangent(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	//Indica como recorrer los elementos
	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		malla->getNumTriangles() * sizeof(unsigned int) * 3, malla->getTriangleIndex(),
		GL_STATIC_DRAW);

	//Carga las texturas
#if CUBE
	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
	normTexId = loadTex("../img/normal.png");
	specTexId = loadTex("../img/specMap.png");
#else
	colorTexId = loadTex("../meshes/color.jpg");
	//emiTexId = loadTex("../img/emissive.png");
	normTexId = loadTex("../meshes/normales.jpg");
	specTexId = loadTex("../meshes/specular.jpg");
#endif
	model = glm::mat4(1.0f); //Matriz identidad

}

void initLights()
{
	lights[0].Amb = glm::vec3(0.0);
	lights[0].Diff = glm::vec3(1.0);
	lights[0].Pos = glm::vec4(0.0, .0, 0.01, 0.0);
	lights[0].Dir = glm::vec4(0.0, 0.0, 0.01f, 0.0);
	lights[0].C = glm::vec3(0.0, 0.0, 0.0);
	lights[0].CosCutOff = -1.0f;// 0.3f;
	lights[0].SpotExponent = 1.0f;

	lights[1].Amb = glm::vec3(0.0);
	lights[1].Diff = glm::vec3(0.0);
	lights[1].Pos = glm::vec4(0.0);
	lights[1].Dir = glm::vec4(0.0);
	lights[1].C = glm::vec3(1.0, 0.0, 0.0);
	lights[1].CosCutOff = 0.0f;
	lights[1].SpotExponent = 0.0f;
}

GLuint loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, //1: numero de cadenas del fichero
		(const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete source; // Liberar el recurso utilizado para cargar el shader

				   //Comprobamos que se compiló bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); // Obtiene si el shader se ha compilado o no
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen; // Longitud de la cadena de error
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); // Reservar el espacio en la cadena de error
		char *logString = new char[logLen]; // Crear el string del error
		glGetShaderInfoLog(shader, logLen, NULL, logString); // Obtener el string con el error
		std::cout << "FILENAME: " << fileName << std::endl;
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteShader(shader);
		std::cout << "Press Return to end the program" << std::endl;
		std::cin.get();
		exit(-1);
	}


	return shader;
}
unsigned int loadTex(const char *fileName)
{
	//Carga textura de fichero
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map) //Si devuelve null es que no se ha cargado la textura
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map); //Reserva espacio y sube textura, primer 0 indica los niveles de detalle, RGBA8: formato interno de la tarjeta gráfica (como se almacenan los datos), RGBA: formato en el que se pasan los datos, Unsigned: como son los datos

	delete[] map; //Borrar de memoria la textura original

	glGenerateMipmap(GL_TEXTURE_2D); //Genera midmaps

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR); //Filtrado trilinear
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Filtrado lineal
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	//Anisotropico si es posible
	if (anisotropico != -1.0f)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropico);

	return texId; //Devuelve el identificador de textura
}

void renderFunc()
{
	scene.RenderLoop();
}

//void renderFunc()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia buffer de color y profundidad
//
//	glUseProgram(program);
//
//	//Subir luces
//	for (int i = 0; i < MAX_LIGHTS; i++)
//	{
//		if (lights[i].uAmb != -1)
//		{
//
//			glUniform3fv(lights[i].uAmb, 1, &lights[i].Amb[0]);
//		}
//		if (lights[i].uDiff != -1)
//		{
//			glm::vec3 ambInt = lights[i].Diff;
//
//			if (i == 0) {
//				ambInt *= lightIntensity;
//			}
//
//			glUniform3fv(lights[i].uDiff, 1, &ambInt[0]);
//		}
//		if (lights[i].uPos != -1)
//		{
//			glm::mat4 lightModel = glm::mat4(1.0);
//
//			if (i == 0)
//			{
//				lightModel = glm::rotate(lightModel, lightYaw, glm::vec3(1.0, 0.0, 0.0));
//				lightModel = glm::rotate(lightModel, lightPitch, glm::vec3(0.0, 1.0, 0.0));
//			}
//
//			glm::vec4 viewPos = view * lightModel * lights[i].Pos;
//
//			glUniform4fv(lights[i].uPos, 1, &viewPos[0]);
//		}
//		if (lights[i].uDir != -1)
//		{
//			glm::vec4 viewDir = glm::transpose(glm::inverse(view)) * lights[i].Dir;
//
//			viewDir.w = 0.0f;
//
//			glUniform4fv(lights[i].uDir, 1, &viewDir[0]);
//		}
//		if (lights[i].uC != -1)
//		{
//			glUniform3fv(lights[i].uC, 1, &lights[i].C[0]);
//		}
//		if (lights[i].uCosCutOff != -1)
//		{
//			glUniform1f(lights[i].uCosCutOff, lights[i].CosCutOff);
//		}
//		if (lights[i].uSpotExponent != -1)
//		{
//			glUniform1f(lights[i].uSpotExponent, lights[i].SpotExponent);
//		}
//	}
//
//	// -> pintado del objeto!!!!
//	//Se suben las variables uniformes
//	glm::mat4 modelView = view * model;
//	glm::mat4 modelViewProj = proj * view * model;
//	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
//
//
//	if (uModelViewMat != -1) // Identificador a la variable uniforme
//		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
//			&(modelView[0][0]));
//	if (uModelViewProjMat != -1)
//		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
//			&(modelViewProj[0][0]));
//	if (uNormalMat != -1)
//		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
//			&(normal[0][0]));
//
//	//Texturas
//	if (uColorTex != -1)
//	{
//		glActiveTexture(GL_TEXTURE0); //Activa texture unit 0
//		glBindTexture(GL_TEXTURE_2D, colorTexId);
//		glUniform1i(uColorTex, 0);
//	}
//	if (uEmiTex != -1)
//	{
//		glActiveTexture(GL_TEXTURE0 + 1); //Activa texture unit 1
//		glBindTexture(GL_TEXTURE_2D, emiTexId);
//		glUniform1i(uEmiTex, 1);
//	}
//	if (uSpecTex != -1)
//	{
//		glActiveTexture(GL_TEXTURE0 + 2); //Activa texture unit 2 (especular)
//		glBindTexture(GL_TEXTURE_2D, specTexId);
//		glUniform1i(uSpecTex, 2);
//	}
//	if (uNormTex != -1)
//	{
//		glActiveTexture(GL_TEXTURE0 + 3); //Activa texture unit 3 (normal)
//		glBindTexture(GL_TEXTURE_2D, normTexId);
//		glUniform1i(uNormTex, 3);
//	}
//
//
//
//	//Activar el vao del objeto
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, malla->getNumTriangles() * 3,
//		GL_UNSIGNED_INT, (void*)0); //Recoge los elementos del buffer de tres en tres para dibujar los triángulos del modelo (nº de triángulos * 3)
//
//	//Segundo cubo
//	glm::mat4 modelView2 = view * model2;
//	glm::mat4 modelViewProj2 = proj * view * model2;
//	glm::mat4 normal2 = glm::transpose(glm::inverse(modelView2));
//
//	if (uModelViewMat != -1) // Identificador a la variable uniforme
//		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
//			&(modelView2[0][0]));
//	if (uModelViewProjMat != -1)
//		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
//			&(modelViewProj2[0][0]));
//	if (uNormalMat != -1)
//		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
//			&(normal2[0][0]));
//
//	glDrawElements(GL_TRIANGLES, malla->getNumTriangles() * 3,
//		GL_UNSIGNED_INT, (void*)0); //Recoge los elementos del buffer de tres en tres para dibujar los triángulos del modelo (nº de triángulos * 3)
//
//	glUseProgram(NULL);
//
//	glutSwapBuffers(); // Swap de los buffers
//}

void resizeFunc(int width, int height)
{

	float n = 0.1f;
	float f = 50.0f;

	proj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, n, f);

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
	std::cout << (*scene.camera.GetModelMatrix())[3].x << " " << (*scene.camera.GetModelMatrix())[3].y << " " << (*scene.camera.GetModelMatrix())[3].z << " " << std::endl;


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







