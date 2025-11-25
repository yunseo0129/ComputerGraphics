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
		//glm::mat4 parent = MatParentTranslate * MatParentRot * MatParentScale;
		MatWorld = MatTranslate * MatRotate * MatScale;
		if (MatWorldParent != nullptr)
		{
			MatWorld = (*MatWorldParent) * MatWorld;
		}

		if (OnMove)
		{
			glm::vec3 currentPos = glm::vec3(MatTranslate[3]);
			glm::vec3 direction = TargetPos - currentPos;
			float distance = glm::length(direction);
			if (distance <= 0.05f)
			{
				MatTranslate[3] = glm::vec4(TargetPos, 1.f);
				OnMove = false;

				if (SecondMove)
				{
					SecondMove = false;
					OnMove = true;
					TargetPos = SecondTargetPos;
				}
			}
			else
			{
				direction = glm::normalize(direction) * 0.1f;
				glm::vec3 newPos = currentPos + direction;
				MatTranslate[3] = glm::vec4(newPos, 1.f);
			}
		}
		else if (Rot)
		{
			if (Up)
			{
				ScaleUp(1.001f);
			}
			else
			{
				ScaleUp(0.999f);
			}
			RotateY(-1.f);
			ParentRotateY(0.4f);
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
	void Reset()
	{
		MatScale = glm::mat4(1.f);
		MatRotate = glm::mat4(1.f);
		MatTranslate = glm::mat4(1.f);
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
	void MoveInit(glm::vec3 _pos)
	{
		MatTranslate = glm::translate(glm::mat4(1.f), _pos);
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
	void GrowY(float speed)
	{
		scaleY += speed;
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(scaleX, scaleY, scaleZ));
	}
	void ParentRotateY(float _angle)
	{
		MatParentRot = glm::rotate(MatParentRot, glm::radians(_angle), glm::vec3(0.f, 1.f, 0.f));
	}
	void ParentScaleUp(float _scale)
	{
		MatParentScale = glm::scale(MatParentScale, glm::vec3(_scale, _scale, _scale));
	}
	void MoveTarget(glm::vec3 _pos)
	{
		OnMove = true;
		TargetPos = _pos;
	}
	void MoveTargetUp(glm::vec3 _pos)
	{
		OnMove = true;
		SecondMove = true;
		glm::vec3 currentPos = glm::vec3(MatTranslate[3]);
		TargetPos = _pos + currentPos / 2.f;
		TargetPos.y += 5.f;
		SecondTargetPos = _pos;
	}
	void MoveTargetDown(glm::vec3 _pos)
	{
		OnMove = true;
		SecondMove = true;
		glm::vec3 currentPos = glm::vec3(MatTranslate[3]);
		TargetPos = _pos + currentPos / 2.f;
		TargetPos.y -= 5.f;
		SecondTargetPos = _pos;
	}
	void MoveRotation(bool onoff, bool isUp = false)
	{
		Up = isUp;
		Rot = onoff;
	}
	void YChange()
	{
		if (YchageUp)
		{
			scaleY += speedd;
			MatScale = glm::scale(glm::mat4(1.f), glm::vec3(scaleX, scaleY, scaleZ));
			if (rangee <= scaleY)
				YchageUp = false;
		}
		else
		{
			scaleY -= speedd;
			MatScale = glm::scale(glm::mat4(1.f), glm::vec3(scaleX, scaleY, scaleZ));
			if (minyy >= scaleY)
				YchageUp = true;
		}
	}
	void SetDel()
	{
		isDelete = true;
		isDontBreak = true;
		scaleY = 0.f;
	}
	void SetDontBreak()
	{
		isDontBreak = true;
	}
	bool IsDel()
	{
		return isDelete;
	}
	bool isWall()
	{
		return isDontBreak;
	}
	void SetParentMat(glm::mat4* _parent)
	{
		MatWorldParent = _parent;
	}
	void SetNose()
	{
		isNose = true;
	}
	bool IsNose()
	{
		return isNose;
	}

	glm::mat4* GetMatWorld()
	{
		return &MatWorld;
	}

protected:
	bool bInitialized = false;
	bool OnMove = false;
	bool Up = false;
	bool Rot = false;
	bool SecondMove = false;
	bool YchageUp = true;
	bool isDelete = false;
	bool isDontBreak = false;
	bool isNose = false;
	float scaleX = 1.f;
	float scaleY = 1.f;
	float scaleZ = 1.f;
	glm::vec3 TargetPos = glm::vec3(0.f);
	glm::vec3 SecondTargetPos = glm::vec3(0.f);
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