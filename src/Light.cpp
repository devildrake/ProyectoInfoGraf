#include "..\Light.h"

Light::Light(glm::vec3 pos, glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LType lType, int number) {
	switch (lType) {
	case DIRECTIONAL:
		SetPosition(pos);
		SetDirection(dir);	
		break;
	case POINT:
		SetPosition(pos);
		break;
	case SPOT:
		SetPosition(pos);
		SetDirection(dir);
		break;
	default:
		break;
	}
	Lambient = ambient;
	Ldiffuse = diffuse;
	Lspecular = specular;
	LightType = lType;
	lightNumber = number;
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant;
	c2 = lineal;
	c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = min;
	MaxAperture = max;
}
void Light::EmptyLight(Shader *shad) {
	std::string variable;

	switch (LightType) {
	case DIRECTIONAL:

		glUniform3f(glGetUniformLocation(shad->Program, "dlight.LAmbiental"), 0,0,0);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Ldiffuse"), 0,0,0);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Lspecular"), 0,0,0);

		break;
	case POINT:

		variable = "plight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".LAmbiental").c_str()), 0,0,0);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldiffuse").c_str()), 0,0,0);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lspecular").c_str()), 0,0,0);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".LAmbiental").c_str()),0,0,0);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldiffuse").c_str()),  0, 0, 0);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lspecular").c_str()),  0, 0, 0);

		break;
	default:
		break;
	}
}

void Light::SetLight(Shader *shad, glm::vec3 CamPos) {
	std::string variable;

	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType){
	case DIRECTIONAL:

		glUniform3f(glGetUniformLocation(shad->Program, "dlight.LAmbiental"), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.pos"), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.dir"), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Ldiffuse"), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z );
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Lspecular"), Lspecular.x, Lspecular.y, Lspecular.z);

		break;
	case POINT:		
		
		variable = "plight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".LAmbiental").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldiffuse").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lspecular").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pos").c_str()), Lpos.x, Lpos.y, Lpos.z);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (variable+".LAmbiental").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldiffuse").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lspecular").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".dir").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMax").c_str()), MaxAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMin").c_str()), MinAperture);

		break;
	default:
		break;
	}	
}

void Light::Rotate(glm::vec3 rotation) {
//opcional
}

void Light::SetDirection(glm::vec3 dir) {
	Ldirection = dir;
}

void Light::SetPosition(glm::vec3 pos) {
	Lpos = pos;
}

glm::vec3 Light::GetColor() {
	glm::vec3 uh;
	return uh;
}