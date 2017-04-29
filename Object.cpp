#include "Object.h"

Object::Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef) {
	GLfloat VertexBufferObject[] = {
		//front
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f , -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		//back
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f , -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		//left
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		//right
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		//down
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		//up
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f

	
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		//LIMPIA LOS BUFFERS DE VERTICES

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);

	this->position = position;
	this->scale = scale;
	incrementoRot = 1;
	start = false;
	rotacionX = rotacionY = 0;
}


Object::~Object() {}

void Object::Draw() {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void Object::Move(vec3 translation) {
	position += translation;
}

void Object::Scale(vec3 scal) {
	scale += scal;
}
void Object::Update(GLFWwindow * window) {
	if (!start) {
		prevFrame = glfwGetTime();
		start = true;
	}
	bool up = glfwGetKey(window, GLFW_KEY_UP);
	bool down = glfwGetKey(window, GLFW_KEY_DOWN);
	bool right = glfwGetKey(window, GLFW_KEY_RIGHT);
	bool left = glfwGetKey(window, GLFW_KEY_LEFT);
	bool rotateXPlus = glfwGetKey(window, GLFW_KEY_KP_2);
	bool rotateXMinus = glfwGetKey(window, GLFW_KEY_KP_8);
	bool rotateYPlus = glfwGetKey(window, GLFW_KEY_KP_6);
	bool rotateYMinus = glfwGetKey(window, GLFW_KEY_KP_4);

	float dt = glfwGetTime() - prevFrame;

	prevFrame = glfwGetTime();

	if (rotateXMinus) {
		rotacionX -= incrementoRot*20*dt;
	}
	else if (rotateXPlus) {
		rotacionX += incrementoRot*20*dt;
	}if (rotateYMinus) {
		rotacionY -= incrementoRot* 20 * dt;
	}
	else if (rotateYPlus){
		rotacionY += incrementoRot* 20 * dt;
	}

	if (up) {
		position.y += incrementoRot*dt;
	}
	else if (down) {
		position.y -= incrementoRot*dt;
	}if (right) {
		position.x += incrementoRot*dt;
	}
	else if (left) {
		position.x -= incrementoRot*dt;
	}

}
mat4 Object::GetModelMatrix() {
	mat4 temp;
	temp = glm::translate(temp, position);
	temp = glm::rotate(temp, radians(rotacionX), vec3(1, 0, 0));
	temp = glm::rotate(temp, radians(rotacionY), vec3(0, 1, 0));
	temp = glm::scale(temp,scale);
	return temp;
}

vec3 Object::GetPosition() {
	return position;
}

void Object::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}