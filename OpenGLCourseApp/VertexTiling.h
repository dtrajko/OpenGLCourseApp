#pragma once

#include <glm/gtc/type_ptr.hpp>


struct VertexTiling
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	float TilingFactor; // obsolete, changed to uniform
};
