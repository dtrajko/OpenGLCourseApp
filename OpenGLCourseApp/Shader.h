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
	GLint GetUniformLocationLightDirection();

	// Omni shadow maps
	GLuint GetUniformLocationOmniLightPos();
	GLuint GetUniformLocationFarPlane();

	GLint GetUniformLocationSpecularIntensity();
	GLint GetUniformLocationShininess();

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLights, unsigned int lightCount);
	void SetSpotLights(SpotLight* spotLights, unsigned int lightCount);

	// Directional light shadow map
	void SetTexture(GLuint textureUnit);
	void SetNormalMap(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* transform);

	// Omni shadow maps
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

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
	GLint uniformFarPlane;

	GLuint uniformLightMatrices[6];

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

	GLint uniformSpecularIntensity;
	GLint uniformShininess;

};
