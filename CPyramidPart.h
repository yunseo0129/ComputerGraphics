#pragma once
#include "Default.h"

class CPyramidPart
{
public:
	CPyramidPart();
	~CPyramidPart();

public:
	void Initialize(eCUBEPOS _pos, glm::mat4* _parent, int _shaderID);
	void Draw();
	void Update();

private:
	bool bInitialized = false;
	bool isBottom = false;
	int shaderProgramID;

	GLuint VAO, VBO, EBO;
	glm::mat4 MatWorld;
	glm::mat4* MatParent;

	VerTex vPoint[4];
};