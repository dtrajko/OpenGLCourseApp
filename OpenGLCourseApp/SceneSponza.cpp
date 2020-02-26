#include "SceneSponza.h"


SceneSponza::SceneSponza()
{
	sceneSettings.cameraPosition = glm::vec3(-4.0f, 10.0f, -0.5f);
	sceneSettings.lightDirection = glm::vec3(1.2f, -14.0f, 1.2f);
	sceneSettings.cameraStartYaw = 0.0f;
	sceneSettings.ambientIntensity = 0.2f;
	sceneSettings.diffuseIntensity = 1.0f;
	sceneSettings.shadowMapWidth = 4096;
	sceneSettings.shadowMapHeight = 4096;
	sceneSettings.shadowSpeed = 0.1f;
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 20.0f, 0.0f);
	sceneSettings.pLight_0_diffuseIntensity = 2.0f;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_1_position = glm::vec3(8.92f, 2.75f, -0.85f);
	sceneSettings.pLight_1_diffuseIntensity = 2.0f;
	sceneSettings.pLight_2_color = glm::vec3(0.0f, 0.0f, 1.0f);
	sceneSettings.pLight_2_position = glm::vec3(10.0f, 2.0f, 10.0f);
	sceneSettings.pLight_2_diffuseIntensity = 2.0f;
	sceneSettings.lightProjectionMatrix = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, 0.1f, 36.0f);

	SetSkybox();
	SetTextures();
	SetupModels();
}

void SceneSponza::SetSkybox()
{
	skyboxFaces.push_back("Textures/skybox_4/right.png");
	skyboxFaces.push_back("Textures/skybox_4/left.png");
	skyboxFaces.push_back("Textures/skybox_4/top.png");
	skyboxFaces.push_back("Textures/skybox_4/bottom.png");
	skyboxFaces.push_back("Textures/skybox_4/back.png");
	skyboxFaces.push_back("Textures/skybox_4/front.png");
	skybox = new Skybox(skyboxFaces);
}

void SceneSponza::SetTextures()
{
}

void SceneSponza::SetupModels()
{
	Model* sponza = new Model();
	sponza->LoadModel("Models/sponza.obj");
	models.insert(std::make_pair("sponza", sponza));
}

void SceneSponza::Update(float timestep, LightManager* lightManager)
{
	// Shadow rotation
	glm::vec3 lightDirection = sceneSettings.lightDirection;
	float lightRadius = abs(lightDirection.x);
	float lightAngle = timestep * sceneSettings.shadowSpeed;
	lightDirection.x = (float)cos(lightAngle) * lightRadius;
	lightDirection.z = (float)sin(lightAngle) * lightRadius;
	lightManager->directionalLight.SetDirection(lightDirection);
}

void SceneSponza::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool shadowPass,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	/* Sponza scene */
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.008f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	models["sponza"]->RenderModel(textureSlots["diffuse"], textureSlots["normal"]);

	if (!shadowPass)
	{
		/* ShadowMap display */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 16.0f, -0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		shaders["main"]->SetTexture(textureSlots["shadow"]);
		shaders["main"]->SetNormalMap(textureSlots["shadow"]);
		materials["dull"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quad"]->RenderMesh();
	}
}

SceneSponza::~SceneSponza()
{
}