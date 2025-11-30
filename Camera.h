#pragma once
#include "Default.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

public:
	static CCamera* GetInstance()
	{
		if (!Instance)
		{
			Instance = new CCamera();
		}
		return Instance;
	}

public:
	void UpdateProjection(float w, float h);
	void UpdateView();
	void Initial();
	
	glm::mat4 GetMatProj() const { return MatProj; }
	glm::mat4 GetMatView() const { return MatView; }
	glm::mat4 GetMatProjView() const;

private:
	void KeyInput();

private:
	static CCamera* Instance;
	float yaw = 0.f;
	float pitch = 0.f;
	POINT CenterPoint = { 400, 300 };
	glm::vec3 vPos = glm::vec3(0.f, 1.7f, 0.f);
	glm::vec3 vRight = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 vUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 vLook = glm::vec3(0.f, 0.f, -1.f);
	glm::mat4 MatProj; // Åõ¿µ
	glm::mat4 MatView; // ºä
};