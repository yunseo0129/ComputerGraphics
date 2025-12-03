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
	void UiModeOn() { UIMode = true; }
	void UiModeOff() { UIMode = false; }
	
	glm::mat4 GetMatProj() const { return MatProj; }
	glm::mat4 GetMatView() const { return MatView; }
	glm::mat4 GetMatProjView() const;
	glm::mat4* GetWorldMatPtr() { return &WorldMat; }

	glm::vec3 GetPos() const { return vPos; }
	glm::vec3 GetLook() const { return vLook; }

private:
	void KeyInput();

private:
	static CCamera* Instance;
	bool UIMode = false;
	float yaw = 0.f;
	float pitch = 0.f;
	float cameraSpeed = 0.05f; // 카메라 이동 속도
	float sensitivity = 0.1f;  // 마우스 감도
	POINT CenterPoint = { 400, 300 }; // 마우스 고정용
	glm::vec3 vPos = glm::vec3(2.f, 1.7f, 2.f); // 카메라 위치 벡터
	glm::vec3 vRight = glm::vec3(1.f, 0.f, 0.f); // 라이트벡터
	glm::vec3 vUp = glm::vec3(0.f, 1.f, 0.f); // 업벡터
	glm::vec3 vLook = glm::vec3(0.f, 0.f, -1.f); // 룩벡터
	glm::mat4 MatProj = glm::mat4(1.f); // 원근투영
	glm::mat4 MatView = glm::mat4(1.f); // 원근뷰
	glm::mat4 MatProjOrthor = glm::mat4(1.f); // 직교투영
	glm::mat4 MatViewOrthor = glm::mat4(1.f); // 직교뷰
	glm::mat4 WorldMat = glm::mat4(1.f);
};