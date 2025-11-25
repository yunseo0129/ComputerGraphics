#pragma once
#include "Shape.h"

class CLine : public CShape
{
public:
	CLine();
	virtual ~CLine();

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID);
	virtual void Draw();
	virtual void Update();
	void SetPoint(glm::vec4 p1, glm::vec4 p2);
	void SetColor(glm::vec3 c) { color = c; }

private:
	GLuint VAO, VBO, EBO;
	bool bInitialized;
	int shaderProgramID;
	glm::vec3 color;
	glm::vec4 vPoint[2];
};