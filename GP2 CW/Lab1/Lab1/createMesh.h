#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"


class storeVertices
{
public: 
	storeVertices(const glm::vec3& position, const glm::vec2& textureCoord, const glm::vec3& meshNormal = glm::vec3(0,0,0))
	{
		this-> _position = position;
		this->_textureCoord = textureCoord;
		this->_meshNormal = meshNormal;
	}


	glm::vec3* FetchPosition() { return &_position; }
	glm::vec2* FetchTextCoords() { return &_textureCoord; }
	glm::vec3* FetchNormal() { return &_meshNormal; }
private:
	glm::vec3 _position;
	glm::vec2 _textureCoord;
	glm::vec3 _meshNormal;
};

struct  sphereCollider

{

sphereCollider() {}

sphereCollider(glm::vec3& position, float colRadius)
	{
		this->_position = position;
	}

	glm::vec3 GetPositon() { return _position; }
	float GetColRadius() { return colRadius; }

	void SetPosition(glm::vec3 position)
	{
		this->_position = position;
	}

	void SetRadius(float colRadius)
	{
		this->colRadius = colRadius;
	}

private:
	glm::vec3 _position;
	float colRadius;

};
class createMesh
{
public:
	createMesh();
	~createMesh();

	void DrawMesh();
	void initialise(storeVertices* theVertices, unsigned int numOfVertices, unsigned int* theIndices, unsigned int numOfIndices);
	void loadInModel(const std::string& nameOfFile);
	void initaliseModel(const IndexedModel& modelForInit);
	void checkSphereStatus(glm::vec3 position, float colRadius);
	glm::vec3 getSpherePos() { return sphere.GetPositon(); }
	float getSphereRadius() { return sphere.GetColRadius(); }

private:
	enum
	{
		POS_OF_VERTEX_BUFFER,
		TEXTCOORD_VERTEX_BUFFER,
		NORMAL_VERTEX_BUFFER,
		INDEX_VERTEX_BUFFER,
		NUM_OF_BUFFERS
	};
	sphereCollider sphere;
	GLuint vertArrayObj;
	GLuint vertArrayBuffers[NUM_OF_BUFFERS]; // Create our array of buffers 
	unsigned int drawAmount; // How much of the VAO we want to draw 


};


