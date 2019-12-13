#pragma once

#include <string>

#include <GL/glut.h>

class Texture {
private:
	GLuint name;
	std::string file_path;
	unsigned char* buffer;
	int width, height, channels;

public:
	Texture(const std::string&);
	~Texture();

	void bind() const;
	void unbind() const;
};
