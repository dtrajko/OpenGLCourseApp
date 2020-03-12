#include "ScenePBR.h"
#include "Terrain.h"
#include "ShaderMain.h"
#include "ShaderWater.h"
#include "ShaderPBR.h"
#include "Renderer.h"
#include "Sphere.h"
#include "ImGuiWrapper.h"


ScenePBR::ScenePBR()
{
	sceneSettings.enableShadows      = true;
	sceneSettings.enableOmniShadows  = false;
	sceneSettings.enablePointLights  = true;
	sceneSettings.enableSpotLights   = true;
	sceneSettings.enableWaterEffects = true;
	sceneSettings.enableSkybox       = true;
	sceneSettings.enableNormalMaps   = true;
	sceneSettings.cameraPosition = glm::vec3(0.0f, 10.0f, 15.0f);
	sceneSettings.cameraStartYaw = -90.0f;
	sceneSettings.cameraMoveSpeed = 5.0f;
	sceneSettings.nearPlane = 0.01f;
	sceneSettings.farPlane = 400.0f;
	sceneSettings.ambientIntensity = 1.0f;
	sceneSettings.diffuseIntensity = 1.0f;
	sceneSettings.lightDirection = glm::vec3(1.2f, -14.0f, 1.2f);
	sceneSettings.lightProjectionMatrix = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, 0.1f, 36.0f);
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 20.0f, 0.0f);
	sceneSettings.pLight_0_diffuseIntensity = 0.0f;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_1_position = glm::vec3(8.92f, 2.75f, -0.85f);
	sceneSettings.pLight_1_diffuseIntensity = 0.0f;
	sceneSettings.pLight_2_color = glm::vec3(0.0f, 0.0f, 1.0f);
	sceneSettings.pLight_2_position = glm::vec3(10.0f, 2.0f, 10.0f);
	sceneSettings.pLight_2_diffuseIntensity = 0.0f;
	sceneSettings.sLight_2_color = glm::vec3(0.0f, 1.0f, 1.0f);
	sceneSettings.sLight_2_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_2_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_2_ambientIntensity = 0.4f;
	sceneSettings.sLight_2_diffuseIntensity = 4.0f;
	sceneSettings.shadowMapWidth = 2048;
	sceneSettings.shadowMapHeight = 2048;
	sceneSettings.shadowSpeed = 0.1f;
	sceneSettings.waterHeight = 0.0f; // 1.0f 5.0f
	sceneSettings.waterWaveSpeed = 0.1f;

	SetSkybox();
	SetTextures();
	SetupModels();

	m_LightPosition = glm::vec3(-9.0f, 24.0f, 6.0f);
	m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Albedo = glm::vec3(1.0f, 0.78f, 0.34f);
	m_AmbientOcclusion = 1.0f;
	m_AmbientIntensity = 0.1f;
}

void ScenePBR::SetSkybox()
{
	skyboxFaces.push_back("Textures/skybox_4/right.png");
	skyboxFaces.push_back("Textures/skybox_4/left.png");
	skyboxFaces.push_back("Textures/skybox_4/top.png");
	skyboxFaces.push_back("Textures/skybox_4/bottom.png");
	skyboxFaces.push_back("Textures/skybox_4/back.png");
	skyboxFaces.push_back("Textures/skybox_4/front.png");
	skybox = new Skybox(skyboxFaces);
}

void ScenePBR::SetTextures()
{
	textures.insert(std::make_pair("sponzaWallDiffuse", new Texture("Textures/sponza_bricks_a_diff.tga")));
	textures.insert(std::make_pair("sponzaCeilDiffuse", new Texture("Textures/sponza_ceiling_a_diff.tga")));
	textures.insert(std::make_pair("sponzaCeilNormal", new Texture("Textures/sponza_ceiling_a_ddn.tga")));
	textures["sponzaWallDiffuse"]->LoadTexture();
	textures["sponzaCeilDiffuse"]->LoadTexture();
	textures["sponzaCeilNormal"]->LoadTexture();

	textures.insert(std::make_pair("rustedIronAlbedo", new Texture("Textures/PBR/rustediron2_basecolor.png")));
	textures.insert(std::make_pair("rustedIronNormal", new Texture("Textures/PBR/rustediron2_normal.png")));
	textures.insert(std::make_pair("rustedIronMetallic", new Texture("Textures/PBR/rustediron2_metallic.png")));
	textures.insert(std::make_pair("rustedIronRoughness", new Texture("Textures/PBR/rustediron2_roughness.png")));
	textures.insert(std::make_pair("rustedIronAmbientOcclusion", new Texture("Textures/PBR/rustediron2_ambient_occlusion.png")));
	textures["rustedIronAlbedo"]->LoadTexture();
	textures["rustedIronNormal"]->LoadTexture();
	textures["rustedIronMetallic"]->LoadTexture();
	textures["rustedIronRoughness"]->LoadTexture();
	textures["rustedIronAmbientOcclusion"]->LoadTexture();
}

void ScenePBR::SetupModels()
{
	Sphere* sphere = new Sphere();
	sphere->CreateMesh();
	meshes.insert(std::make_pair("sphere", sphere));
}

void ScenePBR::Update(float timestep, Camera* camera, LightManager& lightManager, WaterManager* waterManager)
{
	m_LightPosition = camera->GetPosition() + glm::vec3(-5.0f, 5.0f, 0.0f);

	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPosition), -100.0f, 100.0f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
	ImGui::ColorEdit3("Albedo", glm::value_ptr(m_Albedo));
	ImGui::SliderFloat("Ambient Occlusion", &m_AmbientOcclusion, 0.0f, 2.0f);
	ImGui::SliderFloat("Ambient Intensity", &m_AmbientIntensity, 0.0f, 10.0f);
}

void ScenePBR::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms, WaterManager* waterManager)
{
	Renderer::EnableCulling();
}

void ScenePBR::RenderPBR(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::string passType, std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	Renderer::EnableCulling();

	ShaderPBR* shaderPBR = static_cast<ShaderPBR*>(shaders["pbr"]);

	shaderPBR->Bind();

	glm::vec3 lightPositions[4] =
	{
		m_LightPosition,
		m_LightPosition + glm::vec3( 0.0f, -10.0f, 0.0f),
		m_LightPosition + glm::vec3(10.0f,  0.0f,  0.0f),
		m_LightPosition + glm::vec3(10.0f, -10.0f, 0.0f),
	};

	glm::vec3 lightColors[4] =
	{
		m_LightColor,
		m_LightColor,
		m_LightColor,
		m_LightColor,
	};

	shaderPBR->SetLightPositions(lightPositions, 4);
	shaderPBR->SetLightColors(lightColors, 4);

	// Model matrix
	glm::mat4 model;

	for (int v = 0; v <= 5; ++v)
	{
		for (int h = 0; h <= 5; ++h)
		{
			float x = ((float)h - 2.0f) * 2.5f;
			float y = ((float)v - 2.0f) * 2.5f + 8.0f;

			/* Sphere model */
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x, y, 0.0f));
			model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f));
			glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));

			shaderPBR->SetAlbedo(m_Albedo);
			shaderPBR->SetMetallic((float)v / 5.0f);
			shaderPBR->SetRoughness((float)h / 5.0f);
			shaderPBR->SetAmbientOcclusion(m_AmbientOcclusion);
			shaderPBR->SetAmbientIntensity(m_AmbientIntensity);

			textures["rustedIronAlbedo"]->Bind(textureSlots["albedo"]);
			textures["rustedIronNormal"]->Bind(textureSlots["normal"]);
			textures["rustedIronMetallic"]->Bind(textureSlots["metallic"]);
			textures["rustedIronRoughness"]->Bind(textureSlots["roughness"]);
			textures["rustedIronAmbientOcclusion"]->Bind(textureSlots["ao"]);

			shaderPBR->SetAlbedoMap(textureSlots["albedo"]);
			shaderPBR->SetNormalMap(textureSlots["normal"]);
			shaderPBR->SetMetallicMap(textureSlots["metallic"]);
			shaderPBR->SetRoughnessMap(textureSlots["roughness"]);
			shaderPBR->SetAmbientOcclusionMap(textureSlots["ao"]);

			meshes["sphere"]->RenderMesh();

		}
	}
}

void ScenePBR::RenderWater(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms, WaterManager* waterManager)
{
	if (!sceneSettings.enableWaterEffects) return;

	Renderer::EnableCulling();

	ShaderWater* shaderWater = (ShaderWater*)shaders["water"];

	// Model matrix
	glm::mat4 model;
	
	/* Water Tile */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, waterManager->GetWaterHeight(), 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(32.0f, 1.0f, 32.0f));
	
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	waterManager->GetReflectionFramebuffer()->GetColorAttachment()->Bind(textureSlots["reflection"]);
	waterManager->GetRefractionFramebuffer()->GetColorAttachment()->Bind(textureSlots["refraction"]);
	waterManager->GetRefractionFramebuffer()->GetDepthAttachment()->Bind(textureSlots["depth"]);
	shaders["water"]->SetTexture(textureSlots["reflection"]);
	textures["normalMapDefault"]->Bind(textureSlots["normal"]);
	textures["waterDuDv"]->Bind(textureSlots["DuDv"]);
	shaderWater->SetLightColor(LightManager::directionalLight.GetColor());
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["water"]->RenderMesh();
}

ScenePBR::~ScenePBR()
{
}
