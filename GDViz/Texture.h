#pragma once
#include <string>

class Texture {
private:
	unsigned int name;
	std::string file_path;
	unsigned char* buffer;
	int width, height, channels;

public:
	Texture(const std::string&);
	~Texture();

	void bind() const;
	void unbind() const;
};
