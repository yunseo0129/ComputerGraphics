#pragma once
#include "Shape.h"
#include "CubePart.h"

class CSkyBox : public CShape
{
public:
	CSkyBox();
	~CSkyBox();

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID);
	virtual void Draw();
	virtual void Update();

private:
	GLuint VAO, VBO, EBO;
	vector<CCubePart*> vecPart;
};