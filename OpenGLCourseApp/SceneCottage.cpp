#include "SceneCottage.h"

#include "ImGuiWrapper.h"

#include "MeshData.h"
#include "ShaderMain.h"
#include "Sphere.h"


SceneCottage::SceneCottage()
{
	sceneSettings.enableShadows      = true;
	sceneSettings.enableOmniShadows  = true;
	sceneSettings.enablePointLights  = true;
	sceneSettings.enableSpotLights   = true;
	sceneSettings.enableWaterEffects = true;
	sceneSettings.enableSkybox       = true;
	sceneSettings.enableNormalMaps   = true;
	sceneSettings.cameraPosition = glm::vec3(0.0f, 25.0f, 15.0f);
	sceneSettings.cameraStartYaw = -90.0f;
	sceneSettings.cameraMoveSpeed = 4.0f;
	sceneSettings.ambientIntensity = 0.2f;
	sceneSettings.diffuseIntensity = 2.0f;
	sceneSettings.lightDirection = glm::vec3(-0.8f, -1.2f, 0.8f);
	sceneSettings.lightProjectionMatrix = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f, 0.1f, 32.0f);
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 6.0f, 0.0f);
	sceneSettings.pLight_0_diffuseIntensity = 6.0f;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 0.0f, 1.0f);
	sceneSettings.pLight_1_position = glm::vec3(-5.0f, 8.0f, -5.0f);
	sceneSettings.pLight_1_diffuseIntensity = 6.0f;
	sceneSettings.pLight_2_color = glm::vec3(0.0f, 0.0f, 1.0f);
	sceneSettings.pLight_2_position = glm::vec3(10.0f, 2.0f, 10.0f);
	sceneSettings.pLight_2_diffuseIntensity = 6.0f;
	sceneSettings.shadowMapWidth = 1024;
	sceneSettings.shadowMapHeight = 1024;
	sceneSettings.shadowSpeed = 2.0f;
	sceneSettings.waterHeight = 20.0f;
	sceneSettings.waterWaveSpeed = 0.01f;

	SetSkybox();
	SetTextures();
	SetupMeshes();
	SetupModels();
}

void SceneCottage::SetSkybox()
{
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	m_Skybox = new Skybox(skyboxFaces);
}

void SceneCottage::SetTextures()
{
	textures.insert(std::make_pair("brick", new Texture("Textures/brick.png")));
	textures.insert(std::make_pair("crateDiffuse", new Texture("Textures/crate.png")));
	textures.insert(std::make_pair("crateNormal", new Texture("Textures/crateNormal.png")));
	textures.insert(std::make_pair("grass", new Texture("Textures/grass.jpg")));
	textures.insert(std::make_pair("sponzaFloorDiffuse", new Texture("Textures/sponza_floor_a_diff.tga")));
	textures.insert(std::make_pair("sponzaFloorNormal", new Texture("Textures/sponza_floor_a_ddn.tga")));
	textures.insert(std::make_pair("sponzaWallDiffuse", new Texture("Textures/sponza_bricks_a_diff.tga")));
	textures.insert(std::make_pair("sponzaWallNormal", new Texture("Textures/sponza_bricks_a_ddn.tga")));
	textures.insert(std::make_pair("sponzaCeilDiffuse", new Texture("Textures/sponza_ceiling_a_diff.tga")));
	textures.insert(std::make_pair("sponzaCeilNormal", new Texture("Textures/sponza_ceiling_a_ddn.tga")));
}

void SceneCottage::SetupMeshes()
{
	Mesh* cube = new Mesh();
	cube->Create(&MeshData::vertices[0], &MeshData::indices[0], MeshData::vertexCount, MeshData::indexCount);
	meshes.insert(std::make_pair("cube", cube));

	Mesh* quad = new Mesh();
	quad->Create(&MeshData::quadVertices[0], &MeshData::quadIndices[0], MeshData::quadVertexCount, MeshData::quadIndexCount);
	meshes.insert(std::make_pair("quad", quad));

	Mesh* quadLarge = new Mesh();
	quadLarge->Create(&MeshData::floorVertices[0], &MeshData::floorIndices[0], MeshData::floorVertexCount, MeshData::floorIndexCount);
	meshes.insert(std::make_pair("quadLarge", quadLarge));
}

void SceneCottage::SetupModels()
{
	Model* cottage = new Model();
	cottage->LoadModel("Models/cottage.obj");
	models.insert(std::make_pair("cottage", cottage));

	Sphere* sphere = new Sphere();
	sphere->Create();
	meshes.insert(std::make_pair("sphere", sphere));
}

void SceneCottage::Update(float timestep, Window& mainWindow)
{
}

void SceneCottage::UpdateImGui(float timestep, Window& mainWindow, std::map<const char*, float> profilerResults)
{
	glm::vec3 dirLightDirection = m_LightManager->directionalLight.GetDirection();
	glm::vec3 dirLightColor = m_LightManager->directionalLight.GetColor();

	glm::vec3 pLightPos = sceneSettings.pLight_0_position;
	float lightRadius = 6.0;
	float lightAngle = timestep * sceneSettings.shadowSpeed;
	pLightPos.x += (float)cos(lightAngle) * lightRadius;
	pLightPos.z += (float)sin(lightAngle) * lightRadius;
	pLightPos.y += (float)cos(lightAngle * 0.5) * lightRadius * 0.5f;
	m_LightManager->pointLights[0].SetPosition(pLightPos);

	glm::vec3 PL0_Position = m_LightManager->pointLights[0].GetPosition();
	glm::vec3 PL0_Color = m_LightManager->pointLights[0].GetColor();
	float PL0_AmbIntensity = m_LightManager->pointLights[0].GetAmbientIntensity();
	float PL0_DiffIntensity = m_LightManager->pointLights[0].GetDiffuseIntensity();
	glm::vec3 PL1_Position = m_LightManager->pointLights[1].GetPosition();
	glm::vec3 PL1_Color = m_LightManager->pointLights[1].GetColor();
	float PL1_AmbIntensity = m_LightManager->pointLights[1].GetAmbientIntensity();
	float PL1_DiffIntensity = m_LightManager->pointLights[1].GetDiffuseIntensity();

	ImGui::SliderFloat3("DirLight Direction", glm::value_ptr(dirLightDirection), -1.0f, 1.0f);
	ImGui::ColorEdit3("DirLight Color", glm::value_ptr(dirLightColor));
	ImGui::SliderFloat3("PL0 Position", glm::value_ptr(PL0_Position), -20.0f, 20.0f);
	ImGui::ColorEdit3("PL0 Color", glm::value_ptr(PL0_Color));
	ImGui::SliderFloat("PL0 Amb Intensity", &PL0_AmbIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat("PL0 Diff Intensity", &PL0_DiffIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat3("PL1 Position", glm::value_ptr(PL1_Position), -20.0f, 20.0f);
	ImGui::ColorEdit3("PL1 Color", glm::value_ptr(PL1_Color));
	ImGui::SliderFloat("PL1 Amb Intensity", &PL1_AmbIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat("PL1 Diff Intensity", &PL1_DiffIntensity, -20.0f, 20.0f);

	m_LightManager->directionalLight.SetDirection(dirLightDirection);
	m_LightManager->directionalLight.SetColor(dirLightColor);

	m_LightManager->pointLights[0].SetPosition(PL0_Position);
	m_LightManager->pointLights[0].SetColor(PL0_Color);
	m_LightManager->pointLights[0].SetAmbientIntensity(PL0_AmbIntensity);
	m_LightManager->pointLights[0].SetDiffuseIntensity(PL0_DiffIntensity);
	m_LightManager->pointLights[1].SetPosition(PL1_Position);
	m_LightManager->pointLights[1].SetColor(PL1_Color);
	m_LightManager->pointLights[1].SetAmbientIntensity(PL1_AmbIntensity);
	m_LightManager->pointLights[1].SetDiffuseIntensity(PL1_DiffIntensity);
}

void SceneCottage::Render(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	glm::mat4 sceneOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	ShaderMain* shaderMain = (ShaderMain*)shaders["main"];

	// Model matrix
	glm::mat4 model;

	/* Sphere model */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	textures["sponzaCeilDiffuse"]->Bind(textureSlots["diffuse"]);
	textures["normalMapDefault"]->Bind(textureSlots["normal"]);
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["sphere"]->Render();

	/* Cube Left */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.0f, 3.0f, -5.0f)) * sceneOrigin;
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	textures["brick"]->Bind(textureSlots["diffuse"]);
	textures["normalMapDefault"]->Bind(textureSlots["normal"]);
	materials["shiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["cube"]->Render();

	/* Cube Right */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.0f, 2.0f, 0.0f)) * sceneOrigin;
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	textures["crateDiffuse"]->Bind(textureSlots["diffuse"]);
	textures["crateNormal"]->Bind(textureSlots["normal"]);
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["cube"]->Render();

	/* Cube Front */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.0f, 1.0f, 3.0f)) * sceneOrigin;
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	textures["crateDiffuse"]->Bind(textureSlots["diffuse"]);
	textures["crateNormal"]->Bind(textureSlots["normal"]);
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["cube"]->Render();

	/* Cottage */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 20.0f, -5.0f)) * sceneOrigin;
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	models["cottage"]->RenderModel(textureSlots["diffuse"], textureSlots["normal"], sceneSettings.enableNormalMaps);

	if (passType == "main")
	{
		/* Floor */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)) * sceneOrigin;
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaFloorDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaFloorNormal"]->Bind(textureSlots["normal"]);
		materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Floor 2nd */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f)) * sceneOrigin;
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaFloorDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaFloorNormal"]->Bind(textureSlots["normal"]);
		materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Floor 3nd */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f)) * sceneOrigin;
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["grass"]->Bind(textureSlots["diffuse"]);
		materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Wall Right */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 10.0f, 0.0f)) * sceneOrigin;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaWallDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaWallNormal"]->Bind(textureSlots["normal"]);
		materials["shiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		// meshList["quadLarge"]->RenderMesh();

		/* Wall Left */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, 0.0f)) * sceneOrigin;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaWallDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaWallNormal"]->Bind(textureSlots["normal"]);
		materials["shiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Wall Back */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -10.0f)) * sceneOrigin;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaWallDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaWallNormal"]->Bind(textureSlots["normal"]);
		materials["shiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Ceil */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 9.99f, 0.0f)) * sceneOrigin;
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaCeilDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaCeilNormal"]->Bind(textureSlots["normal"]);
		materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* Ceil 2nd */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 19.99f, 0.0f)) * sceneOrigin;
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		textures["sponzaCeilDiffuse"]->Bind(textureSlots["diffuse"]);
		textures["sponzaCeilNormal"]->Bind(textureSlots["normal"]);
		materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quadLarge"]->Render();

		/* ShadowMap display */
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-9.95f, 5.0f, 5.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
		shaderMain->setInt("theTexture", textureSlots["shadow"]);
		shaderMain->setInt("normalMap", textureSlots["shadow"]);
		materials["dull"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quad"]->Render();
	}
}

SceneCottage::~SceneCottage()
{
}
