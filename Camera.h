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
	void SetPosition(const glm::vec3& pos) { vPos = pos; UpdateView(); }
	void SetTarget(const glm::vec3& target) { vTarget = target; UpdateView(); }
	void SetUp(const glm::vec3& up) { vUp = up; UpdateView(); }
	void Initial();
	void ChangePro(bool isOrthor)
	{
		if (isOrthor)
			MatProj = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 0.1f, 100.0f);
		else
			MatProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	}
	void MoveX(float x)
	{
		vPos.x += x;
		vTarget.x += x;
	}
	void MoveY(float y)
	{
		vPos.y += y;
		vTarget.y += y;
	}
	void MoveZ(float z)
	{
		vPos.z += z;
		vTarget.z += z;
	}
	void OnlyMoveZ(float z)
	{
		vPos.z += z;
		if (vPos.z < vTarget.z - 50.f)
		{
			vPos.z = vTarget.z - 50.f;
		}
		else if (vPos.z > vTarget.z + 50.f)
		{
			vPos.z = vTarget.z + 50.f;
		}
	}
	void RotateY(float angle)
	{
		glm::vec4 vec = glm::vec4((vPos - vTarget), 1.f); // 타겟기준에서의 내 위치
		glm::mat4 mat = glm::rotate(glm::mat4(1.f), glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
		vec = mat * vec;
		vPos = vTarget + glm::vec3(vec);
	}
	void SetMini(bool _is)
	{
		isMini = _is;
	}
	bool GetMini() const { return isMini; }

	glm::mat4 GetMatProj() const { return MatProj; }
	glm::mat4 GetMatView() const { return MatView; }
	glm::mat4 GetMatProjView() const;
	glm::mat4* GetWorldMatPtr() { return &WorldMat; }

private:
	static CCamera* Instance;
<<<<<<< Updated upstream
	bool isMini = false;
	glm::vec3 vPos;
	glm::vec3 vTarget;
	glm::vec3 vUp;
	glm::mat4 MatProj; // 투영
	glm::mat4 MatView; // 뷰
	glm::mat4 MatMiniProj;
	glm::mat4 MatMiniView;
=======
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
>>>>>>> Stashed changes
};