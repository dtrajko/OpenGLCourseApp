#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Block.h"
#include "Ring.h"
#include "Scene.h"


const int GIZMO_MODE_TRANSLATE = 0;
const int GIZMO_MODE_SCALE     = 1;
const int GIZMO_MODE_ROTATE    = 2;


class Gizmo
{
public:
	Gizmo();
	void ChangeMode(int mode);
	void ToggleMode();
	void SetSceneObject(SceneObject* sceneObject);
	void Update(glm::vec3 cameraPosition);
	void Render(Shader* shader);
	~Gizmo();

private:
	int m_Mode;

	SceneObject* m_SceneObject;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	EventCooldown m_ToggleMode;

	std::vector<SceneObject*> m_SceneObjects;

	// Translate meshes
	SceneObject* m_Axis_T_X;
	SceneObject* m_Axis_T_Y;
	SceneObject* m_Axis_T_Z;

	SceneObject* m_Arrow_T_X;
	SceneObject* m_Arrow_T_Y;
	SceneObject* m_Arrow_T_Z;

	SceneObject* m_Square_T_XY; // yellow
	SceneObject* m_Square_T_YZ; // cyan
	SceneObject* m_Square_T_ZX; // magenta

	// Rotate meshes
	SceneObject* m_Ring_S_X;
	SceneObject* m_Ring_S_Y;
	SceneObject* m_Ring_S_Z;

	// Scale meshes
	SceneObject* m_Axis_S_X;
	SceneObject* m_Axis_S_Y;
	SceneObject* m_Axis_S_Z;

	SceneObject* m_Handle_S_X;
	SceneObject* m_Handle_S_Y;
	SceneObject* m_Handle_S_Z;

	// Colors
	glm::vec4 m_Color_Red   = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 m_Color_Green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 m_Color_Blue  = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 m_Color_Yellow  = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 m_Color_Cyan    = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_Color_Magenta = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 m_Color_Selected = m_Color_Yellow;
};
