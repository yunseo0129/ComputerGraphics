#pragma once
#include "Shape.h"
#include "CPyramidPart.h"

class CPyramid : public CShape
{
public:
	CPyramid();
	~CPyramid();

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID);
	virtual void Draw();
	virtual void Update();

private:
	GLuint VAO, VBO, EBO;
	vector<CPyramidPart*> vecPart;
};