#include "modelShader.h"
#include <iostream>
#include <fstream>



void modelShader::initialiseShader(const std::string& nameOfFile)
{
	shaderProgram = glCreateProgram(); // create shader program (openGL saves as ref number)
	modelShaders[0] = CreateShaders(LoadInShader("..\\res\\\shader.vert"), GL_VERTEX_SHADER); // create vertex shader
	modelShaders[1] = CreateShaders(LoadInShader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glAttachShader(shaderProgram, modelShaders[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(shaderProgram, 0, "position"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(shaderProgram, 1, "texCoord");
	glBindAttribLocation(shaderProgram, 2, "meshNormal");

	glLinkProgram(shaderProgram); //create executables that will run on the GPU shaders
	CheckForErrors(shaderProgram, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(shaderProgram); //check the entire program is valid
	CheckForErrors(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_ENUM] = glGetUniformLocation(shaderProgram, "transform"); // associate with the location of uniform variable within a program
}

modelShader::modelShader()
{
}


modelShader::~modelShader()
{
	//Destructs shader
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glDetachShader(shaderProgram, modelShaders[i]); 
		glDeleteShader(modelShaders[i]); 
	}
	glDeleteProgram(shaderProgram); 
}


void modelShader::BindShaders()
{
	//Binds shader
	glUseProgram(shaderProgram);
}

void modelShader::Update(const Transform& transform, const createCamera& myCamera)
{
	//Updates shader
	glm::mat4 myModel = myCamera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_ENUM], 1, GLU_FALSE, &myModel[0][0]);
}



GLuint modelShader::CreateShaders(const std::string& text, unsigned int type)
{
	//Creates and returns a shader
	GLuint myShader = glCreateShader(type); //create shader based on specified type

	if (myShader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(myShader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(myShader); //get open gl to compile shader code

	CheckForErrors(myShader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return myShader;
}


std::string modelShader::LoadInShader(const std::string& nameOfFile)
{
	//Used to load in shaders from file
	std::ifstream file;
	file.open((nameOfFile).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << nameOfFile << std::endl;
	}

	return output;
}

void modelShader::CheckForErrors(GLuint theShader, GLuint errFlag, bool program, const std::string& errMsg)
{
	//Checks for errors
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (program)
		glGetProgramiv(theShader, errFlag, &success);
	else
		glGetShaderiv(theShader, errFlag, &success);

	if (success == GL_FALSE)
	{
		if (program)
			glGetProgramInfoLog(theShader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(theShader, sizeof(error), NULL, error);

		std::cerr << errMsg << ": '" << error << "'" << std::endl;
	}
}



