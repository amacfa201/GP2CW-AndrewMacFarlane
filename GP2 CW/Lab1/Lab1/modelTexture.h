#pragma once
#include <string>
#include <GL\glew.h>

class modelTexture
{
public:
	modelTexture();

	void BindTexture(unsigned int unit); // bind upto 32 textures
	void initialiseTexture(const std::string& nameOfFile);
	~modelTexture();

protected:
private:

	GLuint textureManager;
};


