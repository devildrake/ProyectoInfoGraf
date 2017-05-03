#include "material.h"

Material::Material(char *DiffPath, char*SpectPath, float Shini){
	glGenTextures(1, &TextDiff);
	glBindTexture(GL_TEXTURE_2D, TextDiff);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex, heightTex;
	//widthTex = heightTex = 512;
	unsigned char* image = SOIL_load_image(DiffPath, &widthTex, &heightTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glGenTextures(1, &TextSpec);
	glBindTexture(GL_TEXTURE_2D, TextSpec);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex2, heightTex2;

	unsigned char* image2 = SOIL_load_image(SpectPath, &widthTex2, &heightTex2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex2, heightTex2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);

	glBindTexture(GL_TEXTURE_2D, 0);

	Shininess = Shini;
}


Material::~Material(){
}

void Material::SetMaterial(Shader *shad) {
	GLint locTexDiff = glGetUniformLocation(shad->Program, "miMaterial.TexturaDifusa");
	GLint locTexSpec = glGetUniformLocation(shad->Program, "miMaterial.TexturaEspecular");
	glUniform1i(locTexDiff, 0);
	glUniform1i(locTexSpec, 1);
}

void Material::SetShininess(Shader *shad) {
	glUniform1f(glGetUniformLocation(shad->Program, "miMaterial.shininess"), Shininess);
}

void Material::ActivateTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextDiff);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
}
