#include "modelTexture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>


void modelTexture::initialiseTexture(const std::string& nameOfFile)
{
	int textureWidth, textureHeight, numOfComponents; //width, height, and no of components of image
	unsigned char* textureData = stbi_load((nameOfFile).c_str(), &textureWidth, &textureHeight, &numOfComponents, 4); //load the image and store the data

	if (textureData == NULL)
	{
		std::cerr << "texture load failed" << nameOfFile << std::endl;
	}

	glGenTextures(1, &textureManager); // number of and address of textures
	glBindTexture(GL_TEXTURE_2D, textureManager); //bind texture - define type 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Image Data

	stbi_image_free(textureData);
}

modelTexture::modelTexture()
{
	
}

modelTexture::~modelTexture()
{
	glDeleteTextures(1, &textureManager); // number of and address of textures
}

void modelTexture::BindTexture(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31); /// check we are working with one of the 32 textures

	glActiveTexture(GL_TEXTURE0 + unit); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, textureManager); //type of and texture to bind to unit
}
