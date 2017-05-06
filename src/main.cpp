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
#include "..\Mesh.h"
#include "..\Model.h"
#include "..\Object.h"
#include "..\material.h"
#include "..\Light.h"

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window,double xPos, double yPos);
void scroll_callback(GLFWwindow* window,double xOffset, double yOffset);
bool useDirectional, usePoints, useSpots;
#pragma region Clase Camera

class Camera
{
public:
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);
	void DoMovement(GLFWwindow * window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();
	glm::vec3 GetPos();

private:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	GLfloat Deltatime;
	GLfloat Lastframe;
	GLfloat LastMx;
	GLfloat LastMy;
	GLfloat Sensitivity;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;
};

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov)
{
	Lastframe = 0;
	cameraPos = position;	
	cameraFront = direction;
	Sensitivity = sensitivity;
	FOV = fov;

	vec3 directionX = normalize(vec3(0,direction.y,direction.z));
	vec3 z(0,0,1);
	vec3 directionY = normalize(vec3(direction.x, 0, direction.z));


	PITCH = 90 - degrees(acos(dot(vec3(0,1,0),directionX)));
	YAW = 90 - degrees(acos(dot(directionY, z)));
}

glm::vec3 Camera::GetPos() { return cameraPos; }

void Camera::DoMovement(GLFWwindow * window) {

	Deltatime = glfwGetTime() - Lastframe;
	Lastframe = glfwGetTime();

	float cameraSpeed = 0.5f;

		bool backwards = glfwGetKey(window, GLFW_KEY_W);
		bool forward = glfwGetKey(window, GLFW_KEY_S);
		bool left = glfwGetKey(window, GLFW_KEY_A);
		bool right = glfwGetKey(window, GLFW_KEY_D);
		bool goFaster = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		vec3 worldUp = vec3(0, 1, 0);
		if (goFaster) {
			cameraSpeed = 20;
		}

		if (forward) {
			cameraPos += normalize(cameraFront)*cameraSpeed*Deltatime;
		}
		else if (backwards) {
			cameraPos -= normalize(cameraFront)*cameraSpeed*Deltatime;
		}
		if (left) {
			cameraPos -= (cross(worldUp, (normalize(cameraFront))))*cameraSpeed*Deltatime;
		}
		else if (right) {
			cameraPos += (cross(worldUp, (normalize(cameraFront))))*cameraSpeed*Deltatime;
		}
}

void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
		double offsetX, offsetY;
		if (!firstMouse) {
			firstMouse = true;
			LastMx = xpos;
			LastMy= ypos;
		}
		offsetX = xpos - LastMx;
		offsetY = ypos - LastMy;

		LastMx = xpos;
		LastMy = ypos;

		offsetX *= Sensitivity;
		offsetY *= Sensitivity;

		YAW += offsetX;
		PITCH += offsetY;

		PITCH = clamp(PITCH, -89.9f, 89.9f);
		YAW = mod(YAW, 360.0f);


		vec3 dir;

		dir.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
		dir.y = sin(glm::radians(PITCH));
		dir.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));

		cameraFront = normalize(dir);
	}

void Camera::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
			FOV -= yScroll ;
			FOV = clamp(FOV, 1.f, 45.f);
}
mat4 Camera::LookAt() {
		vec3 zaxis = normalize(cameraFront);
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

		translation[3][0] = -cameraPos.x;
		translation[3][1] = -cameraPos.y;
		translation[3][2] = -cameraPos.z;
		translation[3][3] = 1;

		mat4 res = rotation*translation;

		return res;

}
GLfloat Camera::GetFOV() {
	return FOV;
}
#pragma endregion

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

Camera camara(vec3(0, 0, 3), normalize(vec3(0, 0, -3)), 0.04f, 45.0f);

void main() {
	useDirectional = true;
	useSpots = usePoints = false;
	//initalize GLFW

	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	//create a window


	GLFWwindow* window;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);

	//set function when callback

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback); 
	glfwSetScrollCallback(window, scroll_callback);


	//set windows and viewport
	glViewport(0, 0, screenWithd, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	//fondo azul
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	//cargamos los shader

	//Shader shader = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");

	Shader shaderSimple = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");
	Shader shaderPhong = Shader("./src/VertexShaderPhongTexture.vs", "./src/FragmentShaderPhongTexture.fs");
	//Shader shaderDireccional = Shader("./src/phongMaterialVertexShader.vertexshader", "./src/phongDirectionalMaterialFragmentShader.fragmentshader");
	//Shader shaderPuntual = Shader("./src/phongMaterialVertexShader.vertexshader", "./src/phongPuntualMaterialFragmentShader.fragmentshader");
	//Shader shaderFocal = Shader("./src/phongMaterialVertexShader.vertexshader", "./src/phongFocalMaterialFragmentShader.fragmentshader");

	
	Object cajaControlable(vec3(0.5f,0.5f,0.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), Object::cube);
	Light luzDireccional(vec3(0,0,0), vec3(-3.0f, -3.0f, 0), vec3(0.05f, 0.05f, 0.05f), vec3(0.4, 0.4, 0.4), vec3(0.2, 0.2, 0.2), Light::LType::DIRECTIONAL, 0);

	Object cajaLuzPuntual(vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(4.0f, -8.0f, 0.0f), Object::cube);
	Light luzPuntual1(cajaLuzPuntual.GetPosition(),vec3(0,0,0),vec3(0.05f,0.05f,0.05f),vec3(0.5,0.5,0.5),vec3(0.2,0.2,0.2),Light::LType::POINT,0);
	

	Object cajaLuzPuntual2(vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(-4.0f, 4.0f, 0.0f), Object::cube);
	Light luzPuntual2(cajaLuzPuntual2.GetPosition(), vec3(0, 0, 0), vec3(0.05f, 0.05f, 0.05f), vec3(0.5, 0.5, 0.5), vec3(0.2, 0.2, 0.2), Light::LType::POINT, 1);


	Object cajaLuzFocal(vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(-4.0f, 0.0f, -1.0f), Object::cube);
	Light luzFocal1(cajaLuzFocal.GetPosition(), vec3(0, 0, 1), vec3(0.05f, 0.05f, 0.05f), vec3(0.7, 0.7, 0.7), vec3(0.7, 0.7, 0.7), Light::LType::SPOT, 0);
	
	Object cajaLuzFocal2(vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(4.0f, 0.0f, 1.0f), Object::cube);
	Light luzFocal2(cajaLuzFocal2.GetPosition(), vec3(0, 0, 1), vec3(0.05f, 0.05f, 0.05f), vec3(0.7, 0.7, 0.7), vec3(0.7, 0.7, 0.7), Light::LType::SPOT, 1);

	Object cajaLuzFocal3(vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 4.0f, 0.0f), Object::cube);
	Light luzFocal3(cajaLuzFocal3.GetPosition(), vec3(0, 1, 0), vec3(0.05f, 0.05f, 0.05f), vec3(0.7, 0.7, 0.7), vec3(0.7, 0.7, 0.7), Light::LType::SPOT, 2);


	//luzDireccional.SetAtt(1.0f,0.45f,0.075f);
	luzPuntual1.SetAtt(1.0f, 0.35f, 0.44f);
	luzPuntual2.SetAtt(1.0f, 0.35f, 0.44f);
	luzFocal1.SetAtt(1.0f, 0.35f, 0.44f);
	luzFocal2.SetAtt(1.0f, 0.35f, 0.44f);
	luzFocal3.SetAtt(1.0f, 0.35f, 0.44f);

	luzFocal1.SetAperture(20.0f,50.0f);
	luzFocal2.SetAperture(20.0f,50.0f);
	luzFocal3.SetAperture(20.0f, 50.0f);

	//Se habilita el buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	GLint matrizDefID;

	//Bucle de dibujado

	Material miMaterial("./src/Materials/difuso.png", "./src/Materials/especular.png", 230.0f);


	while (!glfwWindowShouldClose(window))
	{
		camara.DoMovement(window);

		mat4 cam; //Vista
		mat4 proj; //Proyeccion

		glfwPollEvents();

		//Establecer el color de fondo (establecido mas arriba)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Se calcula la matriz Proyeccion
		proj = perspective(radians(camara.GetFOV()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		//Se calcula la matriz vista con camara.LookAt();
		cam = camara.LookAt();

		//Model Matrix

		mat4 modelMatrix;

		//Matriz que tendra todas las transformaciones
		mat4 matrizDefinitiva;
		mat4 modelMatrixLuz;
		shaderSimple.USE();

		vec3 colorCajaFocal = vec3(0,0,1);
		vec3 colorCajaPuntual = vec3(0,1,0);

		if (usePoints) {

			matrizDefID = glGetUniformLocation(shaderSimple.Program, "matrizDefinitiva");
			modelMatrixLuz = cajaLuzPuntual.GetModelMatrix();
			matrizDefinitiva = proj*camara.LookAt()*modelMatrixLuz;
			glUniform3f(glGetUniformLocation(shaderSimple.Program, "queColor"), colorCajaPuntual.x, colorCajaPuntual.y, colorCajaPuntual.z);
			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));
			cajaLuzPuntual.Draw();

			matrizDefID = glGetUniformLocation(shaderSimple.Program, "matrizDefinitiva");
			modelMatrixLuz = cajaLuzPuntual2.GetModelMatrix();
			matrizDefinitiva = proj*camara.LookAt()*modelMatrixLuz;
			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));
			cajaLuzPuntual2.Draw();
		
		}

		if (useSpots) {

			glUniform3f(glGetUniformLocation(shaderSimple.Program, "queColor"), colorCajaFocal.x, colorCajaFocal.y, colorCajaFocal.z);
			matrizDefID = glGetUniformLocation(shaderSimple.Program, "matrizDefinitiva");
			modelMatrixLuz = cajaLuzFocal.GetModelMatrix();
			matrizDefinitiva = proj*camara.LookAt()*modelMatrixLuz;
			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));
			cajaLuzFocal.Draw();


			matrizDefID = glGetUniformLocation(shaderSimple.Program, "matrizDefinitiva");
			modelMatrixLuz = cajaLuzFocal2.GetModelMatrix();
			matrizDefinitiva = proj*camara.LookAt()*modelMatrixLuz;
			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));
			cajaLuzFocal2.Draw();


			matrizDefID = glGetUniformLocation(shaderSimple.Program, "matrizDefinitiva");
			modelMatrixLuz = cajaLuzFocal3.GetModelMatrix();
			matrizDefinitiva = proj*camara.LookAt()*modelMatrixLuz;
			glUniformMatrix4fv(matrizDefID, 1, GL_FALSE, glm::value_ptr(matrizDefinitiva));
			cajaLuzFocal3.Draw();

		}

		cajaControlable.Update(window);

		shaderPhong.USE();

		modelMatrix = cajaControlable.GetModelMatrix();

		miMaterial.ActivateTextures();

		miMaterial.SetMaterial(&shaderPhong);

		miMaterial.SetShininess(&shaderPhong);

		glUniformMatrix4fv(glGetUniformLocation(shaderPhong.Program, "model") , 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderPhong.Program, "view"), 1, GL_FALSE, glm::value_ptr(cam));
		glUniformMatrix4fv(glGetUniformLocation(shaderPhong.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

		if(useDirectional)
		luzDireccional.SetLight(&shaderPhong,camara.GetPos());

		if (usePoints) {
			luzPuntual1.SetLight(&shaderPhong, camara.GetPos());
			luzPuntual2.SetLight(&shaderPhong, camara.GetPos());
		}

		if (useSpots) {
			luzFocal1.SetLight(&shaderPhong, camara.GetPos());
			luzFocal2.SetLight(&shaderPhong, camara.GetPos());
			luzFocal3.SetLight(&shaderPhong, camara.GetPos());
		}

		cajaControlable.Draw();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	cajaLuzPuntual.Delete();
	cajaLuzFocal2.Delete();
	cajaLuzFocal3.Delete();
	cajaLuzPuntual2.Delete();
	cajaLuzFocal.Delete();
	cajaLuzPuntual.Delete();
	cajaControlable.Delete();

	glBindTexture(GL_TEXTURE_2D, 0);
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void cursor_callback(GLFWwindow* window, double xPos, double yPos){
		camara.MouseMove(window, xPos, yPos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camara.MouseScroll(window, xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		useDirectional = !useDirectional;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		usePoints = !usePoints;

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		useSpots = !useSpots;

}
