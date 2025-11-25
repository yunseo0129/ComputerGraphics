#pragma once
#include "Default.h"
#include "CRobot.h"

class CCamera1
{
public:
	CCamera1();
	~CCamera1();

public:
	static CCamera1* GetInstance()
	{
		if (!Instance)
		{
			Instance = new CCamera1();
		}
		return Instance;
	}

public:
	void UpdateProjection(float w, float h);
	void UpdateView();
	void SetEnable(bool enable) { isEnable = enable; }
	void SetEnable() { isEnable = !isEnable; }
	void Setview(bool _isFirstPerson)
	{
		isFirstPerson = _isFirstPerson;
	}
	void SetRobot(CRobot* _robot) { robot = _robot; }
	void SetYpos(float y) { fYpos = y; }
	bool GetEnable() const { return isEnable; }
	bool GetFirstPerson() const { return isFirstPerson; }
	glm::mat4 GetMatProj() const { return MatProj; }
	glm::mat4 GetMatView() const { return MatView; }
	glm::mat4 GetMatProjView() const { return MatProj * MatView; }

private:
	static CCamera1* Instance;
	CRobot* robot = nullptr;
	glm::vec3 vPos;
	float fYpos;
	glm::vec3 vTarget;
	glm::vec3 vUp;
	glm::mat4 MatProj; // Åõ¿µ
	glm::mat4 MatView; // ºä
	bool isEnable = false;
	bool isFirstPerson = true;
};