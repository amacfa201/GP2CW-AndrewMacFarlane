#include "createMesh.h"
#include <vector>

//Initialises mesh
void createMesh::initialise(storeVertices* theVertices, unsigned int numOfVertices, unsigned int* theIndices, unsigned int numOfIndices)
{
	IndexedModel myModel;

	for (unsigned int i = 0; i < numOfVertices; i++)
	{
		myModel.positions.push_back(*theVertices[i].FetchPosition()); //Adds positons to vertex list
		myModel.texCoords.push_back(*theVertices[i].FetchTextCoords()); //Adds texture coordinates to vertex list
		myModel.normals.push_back(*theVertices[i].FetchNormal()); //Adds normal to vertex list, used to create lighting effect
	}

	for (unsigned int i = 0; i < numOfIndices; i++)
		myModel.indices.push_back(theIndices[i]);

	initaliseModel(myModel);
}

void createMesh::initaliseModel(const IndexedModel& model)
{

	drawAmount = model.indices.size();

	glGenVertexArrays(1, &vertArrayObj); //generate a vertex array and store it in the VAO
	glBindVertexArray(vertArrayObj); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(NUM_OF_BUFFERS, vertArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];

	glBindBuffer(GL_ARRAY_BUFFER, vertArrayBuffers[POS_OF_VERTEX_BUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertArrayBuffers[TEXTCOORD_VERTEX_BUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertArrayBuffers[NORMAL_VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertArrayBuffers[INDEX_VERTEX_BUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	glBindVertexArray(0); // unbind our VAO
}


createMesh::createMesh()
{
	drawAmount = NULL;
}


void createMesh::loadInModel(const std::string& filename)
{
	IndexedModel model = OBJModel(filename).ToIndexedModel(); //Loads model from file
	initaliseModel(model);
	sphereCollider sphereCollider(); //adds collider
}



createMesh::~createMesh()
{
	glDeleteVertexArrays(1, &vertArrayObj); // delete arrays, essentially removes mesh, used to despawn enemies

}
void createMesh::DrawMesh()
{
	glBindVertexArray(vertArrayObj); //Binds to vertex array object
	glDrawElements(GL_TRIANGLES, drawAmount, GL_UNSIGNED_INT, 0); //Draws elements
	glBindVertexArray(0); //Binds to vertex array
}

void createMesh::checkSphereStatus(glm::vec3 position, float colRadius)
{
	//Sets postion of collider
	sphere.SetPosition(position);
	sphere.SetRadius(colRadius);
}
