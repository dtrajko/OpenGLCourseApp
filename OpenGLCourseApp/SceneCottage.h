#pragma once

#include "Scene.h"


class SceneCottage : public Scene
{

public:
	SceneCottage();
	virtual void Update(float timestep, LightManager* lightManager) override;
	virtual void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool shadowPass,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	~SceneCottage();

private:
	virtual void SetSkybox() override;
	virtual void SetTextures() override;
	virtual void SetupModels() override;
};