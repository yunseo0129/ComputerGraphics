#pragma once
#include "Cube.h"

class CRobot final : public CShape
{
public:
	CRobot();
	~CRobot();

	// CShape을(를) 통해 상속됨
	void Initialize(glm::vec3 _pos, int _shaderID) override;
	void Draw() override;
	void Update();

	void GoStraight();
	void BackStraight();
	void TurnLeft(float angle);
	void TurnRight(float angle);
	void SetSpeed(bool isPlus);
	void SetMirror(vector<CShape*> _shapes)
	{
		vecShape = _shapes;
	}
	void SetMirrorScale(float _scalex, float _scalez)
	{
		mirrorscaleX = _scalex;
		mirrorscaleZ = _scalez;
	}
	bool collisionCheck();

private:
	vector<CCube*> vecPart;
	vector<CShape*> vecShape;
	float fSpeed = 0.02f;
	float mirrorscaleX, mirrorscaleZ;
	float fMoveAngle = 0.f;
	bool isVerse = false;
	bool isMove = false;
};