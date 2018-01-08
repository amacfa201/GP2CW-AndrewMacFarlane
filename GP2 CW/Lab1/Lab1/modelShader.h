#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "createCamera.h"

class modelShader
{
public:
	modelShader();
	void BindShaders(); //set GPU to use our shaders 
	void Update(const Transform& transform, const createCamera& myCamera);
	void initialiseShader(const std::string& nameOfFile);
	
	std::string LoadInShader(const std::string& nameOfFile);
	void CheckForErrors(GLuint theShader, GLuint errFlag, bool program, const std::string&errMsg);
	GLuint CreateShaders(const std::string&text, unsigned int shaderType);
	
	
	~modelShader();

protected:
private:
	
	static const unsigned int NUM_OF_SHADERS = 2;
	//modelShader(const modelShader& _shader);
	//void operator=(const modelShader& other) {}

	enum
	{
		TRANSFORM_ENUM,

		NUM_OF_UNIFORMS
	};



	GLuint shaderProgram;
	GLuint modelShaders[NUM_OF_SHADERS];
	GLuint uniforms[NUM_OF_UNIFORMS];

};

