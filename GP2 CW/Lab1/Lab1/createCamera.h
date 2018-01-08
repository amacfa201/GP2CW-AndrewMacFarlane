#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct createCamera
{
public:
	createCamera()
	{
	}

	void initialiseCamera(const glm::vec3& position, float fieldOfView, float aspectRatio, float nearClipPlane, float farClipPlane)
	{
		this-> _position = position;
		this-> forwardVec = glm::vec3(0.0f, 0.0f, 1.0f);
		this-> upVec = glm::vec3(0.0f, 1.0f, 0.0f);
		this-> projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearClipPlane, farClipPlane);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projectionMatrix * glm::lookAt(_position, _position + forwardVec, upVec);
	}
protected:
private:
	glm::mat4 projectionMatrix;
	glm::vec3 _position;
	glm::vec3 forwardVec;
	glm::vec3 upVec;
};

