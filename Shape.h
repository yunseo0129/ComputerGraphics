#pragma once
#include "Default.h"

class CShape
{
public:
	virtual ~CShape() = 0;

public:
	virtual void Initialize(glm::vec3 _pos, int _shaderID) = 0;
	virtual void Draw() = 0;
	virtual void Update()
	{
		MatWorld = MatTranslate * MatRotate * MatScale;
		if (MatWorldParent != nullptr)
		{
			MatWorld = (*MatWorldParent) * MatWorld;
		}
	}

	glm::vec3 GetPosition() const
	{
		return glm::vec3(MatTranslate[3]);
	}
	glm::mat4 GetMatRotate() const
	{
		return MatRotate;
	}
	void RotateY(float _angle)
	{
		MatRotate = glm::rotate(MatRotate, glm::radians(_angle), glm::vec3(0.f, 1.f, 0.f));
	}
	void RotateZ(float _angle)
	{
		MatRotate = glm::rotate(MatRotate, glm::radians(_angle), glm::vec3(0.f, 0.f, 1.f));
	}
	void RotateX(float _angle)
	{
		MatRotate = glm::rotate(MatRotate, glm::radians(_angle), glm::vec3(1.f, 0.f, 0.f));
	}
	void MoveX(float _move)
	{
		MatTranslate = glm::translate(MatTranslate, glm::vec3(_move, 0.f, 0.f));
	}
	void MoveY(float _move)
	{
		MatTranslate = glm::translate(MatTranslate, glm::vec3(0.f, _move, 0.f));
	}
	void MoveZ(float _move)
	{
		MatTranslate = glm::translate(MatTranslate, glm::vec3(0.f, 0.f, _move));
	}
	void ScaleUp(float _scale)
	{
		scaleX *= _scale;
		scaleY *= _scale;
		scaleZ *= _scale;
		MatScale = glm::scale(MatScale, glm::vec3(scaleX, scaleY, scaleZ));
	}
	void SetScale(float x, float y, float z)
	{
		scaleX = x;
		scaleY = y;
		scaleZ = z;
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(scaleX, scaleY, scaleZ));
	}
	void ParentRotate(float _angle, glm::vec3 axis)
	{
		MatParentRot = glm::rotate(MatParentRot, glm::radians(_angle), axis);
	}
	void ParentScaleUp(float _scale)
	{
		MatParentScale = glm::scale(MatParentScale, glm::vec3(_scale, _scale, _scale));
	}
	void SetParentMat(glm::mat4* _parent)
	{
		MatWorldParent = _parent;
	}
	glm::mat4* GetMatWorld()
	{
		return &MatWorld;
	}
	void SetMap() { isMap = true; }

protected:
	bool bInitialized = false;
	bool isMap = false;
	float scaleX = 1.f;
	float scaleY = 1.f;
	float scaleZ = 1.f;
	glm::mat4 MatParentRot = glm::mat4(1.f);
	glm::mat4 MatParentScale = glm::mat4(1.f);
	glm::mat4 MatParentTranslate = glm::mat4(1.f);
	glm::mat4* MatWorldParent = nullptr;
	glm::mat4 MatWorld = glm::mat4(1.f);
	glm::mat4 MatScale = glm::mat4(1.f);
	glm::mat4 MatRotate = glm::mat4(1.f);
	glm::mat4 MatTranslate = glm::mat4(1.f);
};

inline CShape::~CShape() {}