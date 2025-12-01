#pragma once
#include "Shape.h"
#include "CubePart.h"

class CCube : public CShape
{
public:
	CCube();
	~CCube();

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID);
	virtual void Draw();
	virtual void Update();

	void SetColor(float r, float g, float b);

private:
	GLuint VAO, VBO, EBO;
	vector<CCubePart*> vecPart;
};