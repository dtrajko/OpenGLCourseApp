#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"



class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProgramID();
	GLint GetModelLocation();
	GLint GetViewLocation();
	GLint GetProjectionLocation();
	GLint GetUniformLocationEyePosition();

	// Directional Light
	GLint GetUniformLocationAmbientColor();
	GLint GetUniformLocationAmbientIntensity();
	GLint GetUniformLocationDiffuseIntensity();
	GLint GetUniformLocationDirectionalLightDirection();

	// Omni shadow maps
	GLuint GetUniformLocationOmniLightPos();
	GLuint GetUniformLocationFarPlane();

	GLint GetUniformLocationSpecularIntensity();
	GLint GetUniformLocationShininess();

	// Water shader
	GLint GetUniformLocationReflectionTexture();
	GLint GetUniformLocationRefractionTexture();
	GLint GetUniformLocationDuDvMap();
	GLint GetUniformLocationNormalMap();
	GLint GetUniformLocationDepthMap();
	GLint GetUniformLocationWaterMoveFactor();
	GLint GetUniformLocationCameraPosition();
	GLint GetUniformLocationLightColor();
	GLint GetUniformLocationLightDirection();
	GLuint GetUniformLocationNearPlane();

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight* spotLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);

	// Directional light shadow map
	void SetTexture(GLuint textureUnit);
	void SetNormalMap(GLuint textureUnit);
	void SetDepthMap(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* transform);

	// Omni shadow maps
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	// Water shader samplers
	void SetWater(unsigned int txUnitReflection, unsigned int txUnitRefraction,
		unsigned int txUnitDuDv, unsigned int txUnitDepth);
	void SetClipPlane(glm::vec4 clipPlane);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetWaterMoveFactor(float waterMoveFactor);
	void SetCameraPosition(glm::vec3 cameraPosition);
	void SetLightColor(glm::vec3 lightColor);
	void SetLightDirection(glm::vec3 lightDirection);
	void SetViewMatrix(glm::mat4* viewMatrix);
	void SetProjectionMatrix(glm::mat4* projectionMatrix);

	void Bind();
	void Unbind();
	void ClearShader();
	~Shader();

private:
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint programID, const char* shaderCode, GLenum shaderType);
	void CompileProgram();

	const char* GetShaderTypeNameFromEnum(const GLenum shaderType);

private:
	int pointLightCount;
	int spotLightCount;

	GLuint programID;
	GLint shaderID;

	bool m_Validated = false;

	// Locations of uniform variables
	GLint uniformModel;
	GLint uniformView;
	GLint uniformProjection;
	GLint uniformEyePosition;

	GLint uniformTexture;
	GLint uniformNormalMap;
	GLint uniformDirectionalShadowMap;
	GLint uniformDirectionalLightTransform;

	// omni shadow maps
	GLint uniformOmniLightPos;
	GLint uniformNearPlane;
	GLint uniformFarPlane;

	GLuint uniformLightMatrices[6];

	// Setting the gl_ClipDistance
	GLuint uniformPlane;

	// Water shader sampler2D uniforms
	GLuint uniformWaterMoveFactor;
	GLuint uniformReflectionTexture;
	GLuint uniformRefractionTexture;
	GLuint uniformDuDvMap;
	GLuint uniformDepthMap;
	GLuint uniformCameraPosition;
	GLuint uniformLightColor;
	GLuint uniformLightDirection;

	struct
	{
		GLint uniformColor;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;
		GLint uniformDirection;
	} uniformDirectionalLight;

	GLint uniformPointLightCount;

	struct
	{
		GLint uniformColor;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;
		GLint uniformPosition;
		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLint uniformSpotLightCount;

	struct
	{
		GLint uniformColor;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;
		GLint uniformPosition;
		GLint uniformDirection;
		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;
		GLint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct
	{
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	GLint uniformSpecularIntensity;
	GLint uniformShininess;

};
