#pragma once
#include "Default.h"

class CCubePart
{
public:
	CCubePart();
	~CCubePart();

public:
	void Initialize(eCUBEPOS _pos, glm::mat4* _parent, int _shaderID);
	void Draw();
	void Update();

	void SetSkyBox() { isSkyBox = true; }
	void SetColor(float x, float y, float z);
	void SetMap(bool val) { isMap = val; }

private:
	bool isSkyBox = false;
	bool isMap = true;
	bool bInitialized = false;
	int shaderProgramID;

	GLuint VAO, VBO, EBO;
	glm::mat4 MatWorld;
	glm::mat4* MatParent;

	VerTex vPoint[4];
};