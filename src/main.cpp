//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "..\Shader.h"
#include <SOIL.h>

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
bool paintQuad = false;
bool fade1 = false;
float mixStuff;
float rotacionX,rotacionY = 0.0f;
float gradosRot = 0;
float aumentoRot;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window,double xPos, double yPos);
void scroll_callback(GLFWwindow* window,double xOffset, double yOffset);
void DoMovement(GLFWwindow* window);
mat4 miLookAt(vec3 camPos,vec3 whereToLook, vec3 worldUp);
bool aumentarRotRight, aumentarRotLeft,aumentarUp,aumentarDown;
bool moveForward, moveBackwards, moveRight, moveLeft;
double mouseLastPosX,mouseLastPosY;
bool start;
bool runFaster;
float Pitch, Yaw;
float sensibilidad;
float FOV = 45.0f;
vec3 cameraPos;
vec3 cameraDir;
vec3 cameraRight;
float prevTime = 0;
float cameraSpeed = 0.5f;


mat4 c;
//cout <<"CameraUpA"<< cameraUp.x<<", "<< cameraUp.y <<", "<< cameraUp.z << endl;
mat4 d;
//cout << "CameraUpB" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << endl;
#pragma region Clase Camera
//class Camera
//{
//public:
//	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);
//	void DoMovement(GLFWwindow * window);
//	void MouseMove(GLFWwindow* window, double xpos, double ypos);
//	void MouseScroll(GLFWwindow* window, double xScroll, double yScroll);
//	mat4 LookAt();
//	GLfloat GetFOV();
//
//
//private:
//	vec3 cameraPos;
//	vec3 cameraFront;
//	vec3 cameraUp;
//	GLfloat Deltatime;
//	GLfloat Lastframe;
//	GLfloat LastMx;
//	GLfloat LastMy;
//	GLfloat Sensitivity;
//	GLboolean firstMouse;
//	GLfloat PITCH;
//	GLfloat YAW;
//	GLfloat FOV;
//};
//
//Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov)
//{
//	Lastframe = 0;
//	cameraPos = position;	
//	cameraFront = direction;
//	Sensitivity = sensitivity;
//	FOV = fov;
//}
//
//void Camera::DoMovement(GLFWwindow * window) {
//
//	Deltatime = glfwGetTime() - Lastframe;
//	Lastframe = glfwGetTime();
//
//	float cameraSpeed = 0.5f;
//
//		bool backwards = glfwGetKey(window, GLFW_KEY_W);
//		bool forward = glfwGetKey(window, GLFW_KEY_S);
//		bool left = glfwGetKey(window, GLFW_KEY_A);
//		bool right = glfwGetKey(window, GLFW_KEY_D);
//
//		if (forward) {
//			cameraPos.z -= normalize(cameraFront).z*cameraSpeed*Deltatime;
//		}
//		else if (backwards) {
//			cameraPos.z += normalize(cameraFront).z*cameraSpeed*Deltatime;
//		}
//		if (left) {
//			cameraPos.x += (cross(vec3(0, 1, 0), (normalize(cameraFront)))).x*cameraSpeed*Deltatime;
//		}
//		else if (right) {
//			cameraPos.x -= (cross(vec3(0, 1, 0), (normalize(cameraFront)))).x*cameraSpeed*Deltatime;
//		}
//}
//void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
//		double offsetX, offsetY;
//		if (!start) {
//			YAW = 270.0f;
//			PITCH = 0.0f;
//			start = true;
//			LastMx = cameraPos.x;
//			LastMy= cameraPos.y;
//		}
//		offsetX = cameraPos.x - LastMx;
//		offsetY = cameraPos.y - LastMy;
//
//		LastMx = xpos;
//		LastMy = ypos;
//
//
//		offsetX *= Sensitivity;
//		offsetY *= Sensitivity;
//
//		YAW += offsetX;
//		Pitch -= offsetY;
//
//		Pitch = clamp(PITCH, -89.9f, 89.9f);
//		YAW = mod(YAW, 360.0f);
//
//		cameraFront.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
//		cameraFront.y = sin(glm::radians(PITCH));
//		cameraFront.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
//		cameraFront = normalize(cameraFront);
//	}
//
//void Camera::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
//
//		if (FOV >= 1.0f && FOV <= 45.0f)
//			FOV -= yScroll / 10;
//		if (FOV <= 1.0f)
//			FOV = 1.0f;
//		if (FOV >= 45.0f)
//			FOV = 45.0f;
//}
//mat4 Camera::LookAt() {
//		vec3 zaxis = normalize(cameraFront);
//		vec3 xaxis = normalize(cross(vec3(0, 1, 0), zaxis));
//		vec3 yaxis = cross(zaxis, xaxis);
//		mat4 rotation, translation;
//		rotation[0][0] = xaxis.x;
//		rotation[1][0] = xaxis.y;
//		rotation[2][0] = xaxis.z;
//		rotation[0][1] = yaxis.x;
//		rotation[1][1] = yaxis.y;
//		rotation[2][1] = yaxis.z;
//		rotation[0][2] = zaxis.x;
//		rotation[1][2] = zaxis.y;
//		rotation[2][2] = zaxis.z;
//		rotation[3][3] = 1;
//
//		translation[3][0] = -cameraPos.x;
//		translation[3][1] = -cameraPos.y;
//		translation[3][2] = -cameraPos.z;
//		translation[3][3] = 1;
//
//		mat4 res = rotation*translation;
//
//		return res;
//
//}
//GLfloat Camera::GetFOV() {
//	return FOV;
//}
#pragma endregion

void DrawVao(GLuint programID, GLuint VAO) {
	//establecer el shader
	glUseProgram(programID);

	//pitar el VAO
	glBindVertexArray(VAO);
	if (!paintQuad) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}

void PrintAMatrix4(mat4 a) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "a[" << i << "][" << j << "] = " << a[i][j] << endl;

		}
	}
}

void PrintAndCompareMatrix4(mat4 a, mat4 b) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "a[" << i << "][" << j << "] = " << a[i][j] << endl;
			cout << "b[" << i << "][" << j << "] = " << b[i][j] << endl;
		}
	}
}

mat4 GenerateModelMatrix(vec3 aTranslation, vec3 aRotation, vec3 CubesPosition, float aRot) {
	mat4 temp;
	temp = translate(temp, aTranslation);
	temp = translate(temp, CubesPosition);
	temp = rotate(temp, radians(aRot), aRotation);

	return temp;
}

void DoMovement(GLFWwindow* window){
	float dt = glfwGetTime() - prevTime;
	prevTime = glfwGetTime();
	if (glfwGetKey(window, GLFW_KEY_W)) {
		cameraPos += cameraDir*dt*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		cameraPos -= cameraDir*dt*cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		cameraPos += cameraRight*dt*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D)) {
		cameraPos -= cameraRight*dt*cameraSpeed;
	}
}

void main() {
	runFaster = false;
	sensibilidad = 0.04f;
	start = false;
	mixStuff = 0.0f;
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
	//TODO

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	//create a window
	//TODO
	GLFWwindow* window;



	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	//TODO

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//set function when callback
	//TODO

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback); 
	glfwSetScrollCallback(window, scroll_callback);




	//set windows and viewport
	//TODO
	glViewport(0, 0, screenWithd, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	//fondo
	glClearColor(0.0, 0.0, 1.0, 1.0);


	//TODO


	//cargamos los shader

	//Shader shader = Shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	Shader shader = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");

	//GLuint programID = shader.Program;

	//GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	// Definir el buffer de vertices
	//Reserva de memoria

	//GLfloat traingleVertex[] = {
	//-0.5f,-0.5f,0,
	//0.5f,0.5f,0,
	//-0.5f,0.5f,0,
	//0.5f,-0.5f,0
	//};

	//VBO
	//GLfloat VertexBufferObject[] = {
	//	0.5f,  0.5f, 0.0f,  // Top Right
	//	0.5f, -0.5f, 0.0f,  // Bottom Right
	//	-0.5f, -0.5f, 0.0f,  // Bottom Left
	//	-0.5f,  0.5f, 0.0f   // Top Left 
	//};

	//GLfloat VertexBufferObject[] = {
	//	// Positions          // Colors           // Texture Coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	//};

	GLfloat VertexBufferObject[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};


	//Numbuffer = cantidad de buffers a generar;

	//Borrado
	//glDeleteBuffers(GLsizei n, const GLuint* ids);

	//-VBO

	//EBO

	//GLuint IndexBufferObject[]{
	//	0,1,2,
	//	1,3,0 };

	//GLuint IndexBufferObject[]{
	//	2,0,3,
	//	2,1,0
	//};




	//-EBO


	// Definir el EBO

	// Crear los VBO, VAO y EBO

	GLuint VAO, /*EBO,*/ VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_DYNAMIC_DRAW);


	/*	glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBufferObject), IndexBufferObject, GL_DYNAMIC_DRAW);
		*/

		//Propiedades

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		//LIMPIA LOS BUFFERS DE VERTICES

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);



	//reservar memoria para el VAO, VBO y EBO



	//Establecer el objeto
	//Declarar el VBO y el EBO

	//Enlazar el buffer con openGL

	//Establecer las propiedades de los vertices

	//liberar el buffer

	//liberar el buffer de vertices


	//GLint loc = glGetUniformLocation(shader.Program, "time");


	GLuint texture1, texture2;

	//GLint matProjID,matViewID,matModelID;
	GLint matrizDefID;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex, heightTex;
	//widthTex = heightTex = 512;
	unsigned char* image = SOIL_load_image("./src/texture.png", &widthTex, &heightTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex2, heightTex2;

	unsigned char* image2 = SOIL_load_image("./src/PablitoSatan.png", &widthTex2, &heightTex2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex2, heightTex2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);

	glBindTexture(GL_TEXTURE_2D, 0);

	aumentoRot = 0.05f;
	
	glEnable(GL_DEPTH_TEST);
	//bucle de dibujado

	cameraPos = vec3(0,0,3);
	cameraDir = normalize(vec3(0,0,0)- cameraPos);
	//cameraDir = vec3(0, 0, -1);
	cameraRight = normalize(cross(vec3(0, 1, 0), cameraDir));

	vec3 cameraUp = normalize(cross(cameraDir, cameraRight));

	moveForward = moveBackwards = moveRight = moveLeft = false;
	Pitch = 0;

	while (!glfwWindowShouldClose(window))
	{
		cameraRight = normalize(cross(vec3(0, 1, 0), cameraDir));

		c = miLookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		d = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		if (c == d) {
			cout << "Confirmamos \n";
		}

		if (runFaster) {
			cameraSpeed = 2;
		}
		else {
			cameraSpeed = 0.5f;
		}

		DoMovement(window);

		//vec3 cameraDir = normalize(vec3(0,0,0)- cameraPos);



		//mat4 finalMatrix; //Modelo

		mat4 cam; //Vista
		mat4 proj; //Proyeccion


		//mat4 proj = perspective(60, screenWithd/screenHeight, 0.1f, 100);


		if (gradosRot > 360 || gradosRot < -360) {
			gradosRot = 0;
		}

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

		/*if (loc != -1)
		{
		glUniform1f(loc, glfwGetTime());
		}*/

		glfwPollEvents();

		//matModelID = glGetUniformLocation(shader.Program, "matrizModel");
		//matProjID = glGetUniformLocation(shader.Program, "matrizProj");
		//matViewID = glGetUniformLocation(shader.Program, "matrizView");
		matrizDefID = glGetUniformLocation(shader.Program, "matrizDefinitiva");

		//Establecer el color de fondo
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		GLint locTex = glGetUniformLocation(shader.Program, "ourTexture");
		GLint locTex2 = glGetUniformLocation(shader.Program, "ourTexture2");
		GLint mixID = glGetUniformLocation(shader.Program, "mixStuff");
		shader.USE();
		
		glUniform1f(mixID, mixStuff);


		//glClearColor(0, 1, 0, 0);

		//DrawVao(shader.Program,VAO);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_POLYGON, 6, GL_UNSIGNED_INT, 0);


		//glBindVertexArray(0);

		//glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(locTex,0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(locTex2, 1);
		
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		
		//pintar con lineas
		//pintar con triangulos

		//if (moveForward) {
		//	cameraPos.z -= normalize(cameraDir).z*cameraSpeed*dt;
		//}
		//else if (moveBackwards) {
		//	cameraPos.z += normalize(cameraDir).z*cameraSpeed*dt;
		//}
		//if (moveLeft) {
		//	cameraPos.x+= normalize(cameraRight).x*cameraSpeed*dt;
		//}
		//else if (moveRight) {
		//	cameraPos.x -= normalize(cameraRight).x*cameraSpeed*dt;
		//}

		if (aumentarRotLeft) {
			rotacionY-=aumentoRot;
		}
		else if (aumentarRotRight) {
			rotacionY+= aumentoRot;
		}

		if (aumentarUp) {
			rotacionX -= aumentoRot;
		}
		else if (aumentarDown) {
			rotacionX += aumentoRot;
		}

		if (fade1) {
			if (mixStuff >= 0&&mixStuff<1) {
				mixStuff += 0.01f;
			}
		}
		else {  
			if (mixStuff>0.01f) {
				mixStuff -= 0.01f;
			}
		}

		proj = perspective(FOV, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		cam = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		//glUniformMatrix4fv(matProjID, 1, GL_FALSE, glm::value_ptr(proj));
		//glUniformMatrix4fv(matViewID, 1, GL_FALSE, glm::value_ptr(cam));

		for (int i = 0; i < 10; i++) {
			//Model Matrix
			mat4 modelMatrix;
			if (i == 0) {
				modelMatrix = translate(modelMatrix, CubesPositionBuffer[0]);
				modelMatrix = rotate(modelMatrix, radians(rotacionX), vec3(1,0,0));
				modelMatrix = rotate(modelMatrix, radians(rotacionY),vec3(0,1,0));
			}
			else {
				float rotot = glfwGetTime() * 100;
				rotot = (int)rotot % 360;
				modelMatrix = GenerateModelMatrix(vec3(0.0f), vec3(1, 0.5f, 0), CubesPositionBuffer[i], rotot);
			}
			//glUniformMatrix4fv(matModelID, 1, GL_FALSE, glm::value_ptr(matriz));

			mat4 matrizDefinitiva;

			matrizDefinitiva = proj*cam*modelMatrix;

			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
mat4 miLookAt(vec3 camPos, vec3 whereToLook, vec3 cameraUp) {

	vec3 zaxis = normalize(  camPos- whereToLook);

	vec3 xaxis = normalize(cross(vec3(0, 1, 0), zaxis));
	vec3 yaxis = cross(zaxis, xaxis);

	mat4 rotation, translation;

	rotation[0][0] = xaxis.x;
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x;
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x;
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;
	rotation[3][3] = 1;

	translation[3][0] = -camPos.x;
	translation[3][1] = -camPos.y;
	translation[3][2] = -camPos.z;
	translation[3][3] = 1;

	return rotation*translation;
}

void cursor_callback(GLFWwindow* window, double xPos, double yPos){
	double offsetX, offsetY;
	if (!start) {
		Yaw = 270.0f;
		Pitch = 0.0f;
		start = true;
		mouseLastPosX = xPos;
		mouseLastPosY = yPos;
	}
		offsetX = xPos - mouseLastPosX;
		offsetY = yPos - mouseLastPosY;

		mouseLastPosX = xPos;
		mouseLastPosY = yPos;
		

		offsetX *= sensibilidad;
		offsetY *= sensibilidad;

		Yaw += offsetX;
		Pitch -= offsetY;

		Pitch = clamp(Pitch,-89.9f,89.9f);
		Yaw = mod(Yaw,360.0f);

		vec3 front;

		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraDir = normalize(front);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yoffset/10;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_LEFT_SHIFT&&action == GLFW_PRESS) {
		runFaster = true;
	}

	if (key == GLFW_KEY_E&&action == GLFW_PRESS) {
		PrintAndCompareMatrix4(c,d);
	}

	if (key == GLFW_KEY_UP&&action == GLFW_PRESS) {
		aumentarUp = true;
	}

	if (key == GLFW_KEY_UP&&action == GLFW_RELEASE) {
		aumentarUp = false;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS) {
		aumentarDown = true;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_RELEASE) {
		aumentarDown = false;
	}

	if (key == GLFW_KEY_RIGHT&&action == GLFW_PRESS) {
		aumentarRotRight = true;
	}
	else if (key == GLFW_KEY_RIGHT&&action == GLFW_RELEASE) {
		aumentarRotRight = false;
	}

	if (key == GLFW_KEY_LEFT&&action == GLFW_PRESS) {
		aumentarRotLeft = true;
	}
	else if (key == GLFW_KEY_LEFT&&action == GLFW_RELEASE) {
		aumentarRotLeft = false;
	}

	if (key == GLFW_KEY_1&&action == GLFW_PRESS) {
		fade1 = true;
	}
	else if (key == GLFW_KEY_2&&action == GLFW_PRESS) {
		fade1 = false;
	}

}
