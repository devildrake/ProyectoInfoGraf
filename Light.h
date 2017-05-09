#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>
#include "Shader.h"
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
//using namespace glm;

class Light {
public:
	enum LType {
		DIRECTIONAL = 0,
		POINT = 1,
		SPOT = 2
	};
	Light(glm::vec3 pos, glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LType lType, int number);
	void SetAtt(float constant, float lineal, float quadratic);
	void SetLight(Shader *shad, glm::vec3 CamPos);
	void EmptyLight(Shader *shad);
	void Rotate(glm::vec3 rotation);
	void SetDirection(glm::vec3 dir);
	void SetAperture(float min, float max);
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetColor();

private:
	glm::vec3 Ldirection;
	glm::vec3 Lpos;
	glm::vec3 Lambient;
	glm::vec3 Ldiffuse;
	glm::vec3 Lspecular;
	LType LightType;
	float MinAperture;
	float MaxAperture;
	float c1, c2, c3;
	int lightNumber;
};

