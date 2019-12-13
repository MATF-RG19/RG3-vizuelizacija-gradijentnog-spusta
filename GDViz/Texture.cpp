#include <GL/glut.h>

#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string& file_path)
	: file_path(file_path), buffer(nullptr),
	width(0), height(0), channels(0) {

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(file_path.c_str(), &width, &height, &channels, 3);

	glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_RGB, // internalformat
		width, height,
		0, // must be zero
		GL_RGB, // must match to internalformat
		GL_UNSIGNED_BYTE, buffer
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer)
		stbi_image_free(buffer);
}

Texture::~Texture() {
	glDeleteTextures(1, &name);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, name);
}

void Texture::unbind() const {
}
