#pragma once

#include "Light.h"


class DirectionalLight : public Light
{

public:
	DirectionalLight();

	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
		bool enabled, glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction);
	void UseLight(GLint enabledLocation, GLint ambientColorLocation, GLint ambientIntensityLocation,
		GLint diffuseIntensityLocation, GLint directionLocation);
	inline glm::vec3 GetDirection() const { return m_Direction; };
	inline glm::vec3 GetPosition() const { return -m_Direction; };
	inline void SetDirection(glm::vec3 direction) { m_Direction = direction; };
	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

protected:
	glm::vec3 m_Direction;


};
