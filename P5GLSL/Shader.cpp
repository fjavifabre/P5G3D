#include "Shader.h"


//#include <windows.h> //TODO: check linux comp

#include <fstream>
#include <exception>
#include <string>


Shader::Shader(const char* vertex, const char* fragment)
{
	// Compila los shaders
	vshader = loadShader(vertex, GL_VERTEX_SHADER);
	fshader = loadShader(fragment, GL_FRAGMENT_SHADER);

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

void Shader::render(std::list<LightValues> &lightV, glm::mat4 &view)
{
	glUseProgram(program);


	const float lightIntensity = 1.0f, lightPitch = 0.0f, lightYaw = 0.0f; //TODO: check

	//Subir luces
	//for (int i = 0; i < MAX_LIGHTS; i++)
	int i = 0;
	for (LightValues values : lightV)
	{

		if (lights[i].uAmb != -1)
		{

			glUniform3fv(lights[i].uAmb, 1, &values.Amb[0]);
		}
		if (lights[i].uDiff != -1)
		{
			glm::vec3 ambInt = values.Diff;

			if (i == 0) {
				ambInt *= lightIntensity;
			}

			glUniform3fv(lights[i].uDiff, 1, &ambInt[0]);
		}
		if (lights[i].uPos != -1)
		{
			glm::mat4 lightModel = glm::mat4(1.0);

			if (i == 0)
			{
				lightModel = glm::rotate(lightModel, lightYaw, glm::vec3(1.0, 0.0, 0.0));
				lightModel = glm::rotate(lightModel, lightPitch, glm::vec3(0.0, 1.0, 0.0));
			}

			glm::vec4 viewPos = view * lightModel * values.Pos;

			glUniform4fv(lights[i].uPos, 1, &viewPos[0]);
		}
		if (lights[i].uDir != -1)
		{
			glm::vec4 viewDir = glm::transpose(glm::inverse(view)) * values.Dir;

			viewDir.w = 0.0f;

			glUniform4fv(lights[i].uDir, 1, &viewDir[0]);
		}
		if (lights[i].uC != -1)
		{
			glUniform3fv(lights[i].uC, 1, &values.C[0]);
		}
		if (lights[i].uCosCutOff != -1)
		{
			glUniform1f(lights[i].uCosCutOff, values.CosCutOff);
		}
		if (lights[i].uSpotExponent != -1)
		{
			glUniform1f(lights[i].uSpotExponent, values.SpotExponent);
		}

		i++;
	}

	/*

	// -> pintado del objeto!!!!
	//Se suben las variables uniformes
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));


	if (uModelViewMat != -1) // Identificador a la variable uniforme
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal[0][0]));

	//Texturas
	if (uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0); //Activa texture unit 0
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1); //Activa texture unit 1
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}
	if (uSpecTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2); //Activa texture unit 2 (especular)
		glBindTexture(GL_TEXTURE_2D, specTexId);
		glUniform1i(uSpecTex, 2);
	}
	if (uNormTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3); //Activa texture unit 3 (normal)
		glBindTexture(GL_TEXTURE_2D, normTexId);
		glUniform1i(uNormTex, 3);
	}



	//Activar el vao del objeto
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, malla->getNumTriangles() * 3,
		GL_UNSIGNED_INT, (void*)0); //Recoge los elementos del buffer de tres en tres para dibujar los triángulos del modelo (nº de triángulos * 3)

	//Segundo cubo
	glm::mat4 modelView2 = view * model2;
	glm::mat4 modelViewProj2 = proj * view * model2;
	glm::mat4 normal2 = glm::transpose(glm::inverse(modelView2));

	if (uModelViewMat != -1) // Identificador a la variable uniforme
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView2[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj2[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal2[0][0]));

	glDrawElements(GL_TRIANGLES, malla->getNumTriangles() * 3,
		GL_UNSIGNED_INT, (void*)0); //Recoge los elementos del buffer de tres en tres para dibujar los triángulos del modelo (nº de triángulos * 3)

	glUseProgram(NULL);


	glutSwapBuffers(); // Swap de los buffers

	*/
}

GLuint Shader::loadShader(const char *fileName, GLenum type)
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

		//std::cout << "FILENAME: " << fileName << std::endl;
		//std::cout << "Error: " << logString << std::endl;

		delete logString;
		glDeleteShader(shader);

		//std::cout << "Press Return to end the program" << std::endl;
		//std::cin.get();
		//exit(-1);

		//Throw exception
		throw std::runtime_error("File not found");

	}


	return shader;
}

char * Shader::loadStringFromFile(const char *fileName, unsigned int &fileLen)
{
	//Se carga el fichero
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file) return 0;

	//Se calcula la longitud del fichero
	file.seekg(0, std::ios::end);
	fileLen = unsigned int(file.tellg());
	file.seekg(std::ios::beg);

	//Se lee el fichero
	char *source = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		source[i] = char(file.get());
		if (!file.eof()) i++;
		else fileLen = i;
	}
	source[fileLen] = '\0';
	file.close();

	return source;
}



Shader::~Shader()
{
	//Liberar shaders y programa
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);
}
