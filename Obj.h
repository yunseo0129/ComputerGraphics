#pragma once
#include "Shape.h"

class CObj : public CShape
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID) override;
	virtual void Draw() override;
	virtual void Update() override;

	void GetMatWorld(glm::mat4& outMat) const
	{
		outMat = MatWorld;
	}
	void SetParentMat(glm::mat4* _parent)
	{
		MatOrbit = _parent;
	}
	void SetEarthMat(glm::mat4* _earth)
	{
		MatEarth = _earth;
	}
	void SetObj(const char* _objFile);
	void SetSmall()
	{
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(0.02f, 0.02f, 0.02f));
	}
	void SetCenter()
	{
		isCenter = true;
	}
	void SetStar()
	{
		isSmall = true;
	}
	void SetRadius(float _radius)
	{
		if (MatTranslate[3][0] < 0.f)
			MatTranslate = glm::translate(glm::mat4(1.f), glm::vec3(-_radius, -0.7f, 0.f));
		else if (MatTranslate[3][0] > 0.f)
			MatTranslate = glm::translate(glm::mat4(1.f), glm::vec3(_radius, -0.7f, 0.f));
		else if (MatTranslate[3][0] == 0.f)
			MatTranslate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -0.7f, -_radius));
	}

private:
	void read_newline(char* str) {
		char* pos;
		if ((pos = strchr(str, '\n')) != NULL)
			*pos = '\0';
	}

private:
	bool bInitialized = false;
	bool isCenter = false;
	bool isSmall = false;
	int shaderProgramID;
	int iVertexCount = 0;
	int iFaceCount = 0;

	GLuint VAO, VBO, EBO;
	glm::mat4* MatOrbit;
	glm::mat4* MatEarth;
	VerTex* pVertex;
	Face* pIndices;
};