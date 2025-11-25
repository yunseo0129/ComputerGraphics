#include "CRobot.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Camera1.h"

CRobot::CRobot()
{
}

CRobot::~CRobot()
{
}

void CRobot::Initialize(glm::vec3 _pos, int _shaderID)
{
	MatTranslate = glm::translate(MatTranslate, _pos);

	CCube* head = new CCube;
	head->SetRobotPart();
	head->Initialize(glm::vec3(0.f, 10.f, 0.f), _shaderID);
	head->SetScale(1.f, 1.f, 1.f);
	head->SetParentMat(&MatWorld);
	head->SetColor(0.1f, 0.1f, 0.1f);
	vecPart.push_back(head);

	CCube* nose = new CCube;
	nose->SetRobotPart();
	nose->Initialize(glm::vec3(0.f, 10.4f, 1.f), _shaderID);
	nose->SetScale(0.25f, 0.25f, 0.4f);
	nose->SetParentMat(&MatWorld);
	nose->SetColor(0.4f, 0.4f, 0.4f);
	nose->SetNose();
	vecPart.push_back(nose);

	CCube* body = new CCube;
	body->SetRobotPart();
	body->Initialize(glm::vec3(0.f, 5.f, 0.f), _shaderID);
	body->SetScale(1.f, 2.5f, 0.6f);
	body->SetParentMat(&MatWorld);
	body->SetColor(0.3f, 0.3f, 0.3f);
	vecPart.push_back(body);

	CCube* LReg = new CCube;
	LReg->SetRobotPart();
	LReg->Initialize(glm::vec3(-0.5f, 5.f, 0.f), _shaderID);
	LReg->SetScale(0.5f, 2.5f, 0.6f);
	LReg->RotateX(180.f);
	LReg->SetParentMat(&MatWorld);
	LReg->SetColor(0.7f, 0.7f, 0.7f);
	vecPart.push_back(LReg);

	CCube* RLeg = new CCube;
	RLeg->SetRobotPart();
	RLeg->Initialize(glm::vec3(0.5f, 5.f, 0.f), _shaderID);
	RLeg->SetScale(0.5f, 2.5f, 0.6f);
	RLeg->RotateX(180.f);
	RLeg->SetParentMat(&MatWorld);
	RLeg->SetColor(0.6f, 0.6f, 0.6f);
	vecPart.push_back(RLeg);

	CCube* LArm = new CCube;
	LArm->SetRobotPart();
	LArm->Initialize(glm::vec3(-1.5f, 10.f, 0.f), _shaderID);
	LArm->SetScale(0.5f, 2.5f, 0.6f);
	LArm->RotateX(180.f);
	LArm->SetParentMat(&MatWorld);
	LArm->SetColor(0.5f, 0.5f, 0.5f);
	vecPart.push_back(LArm);

	CCube* RArm = new CCube;
	RArm->SetRobotPart();
	RArm->Initialize(glm::vec3(1.5f, 10.f, 0.f), _shaderID);
	RArm->SetScale(0.5f, 2.5f, 0.6f);
	RArm->RotateX(180.f);
	RArm->SetParentMat(&MatWorld);
	RArm->SetColor(0.7f, 0.7f, 0.7f);
	vecPart.push_back(RArm);
}

void CRobot::Draw()
{
	for (CCube* part : vecPart)
	{
		if (CCamera::GetInstance()->GetMini() || !CCamera1::GetInstance()->GetEnable())
		{
			part->Draw();
		}
		else if (CCamera1::GetInstance()->GetFirstPerson())
		{
			if (!part->IsNose())
				part->Draw();
		}
		else
		{
			part->Draw();
		}
	}
}

void CRobot::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		GoStraight();
		isMove = true;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		BackStraight();
		isMove = true;
	}
	else
	{
		isMove = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		TurnLeft(2.f);
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		TurnRight(2.f);
	}

	if (isMove)
	{
		if (isVerse)
		{
			fMoveAngle -= 1.f;
			vecPart[3]->RotateX(-1.f);
			vecPart[4]->RotateX(1.f);
			vecPart[5]->RotateX(1.f);
			vecPart[6]->RotateX(-1.f);
			if (fMoveAngle <= -30.f)
			{
				isVerse = false;
			}
		}
		else
		{
			fMoveAngle += 1.f;
			vecPart[3]->RotateX(1.f);
			vecPart[4]->RotateX(-1.f);
			vecPart[5]->RotateX(-1.f);
			vecPart[6]->RotateX(1.f);
			if (fMoveAngle >= 30.f)
			{
				isVerse = true;
			}
		}
	}
	else
	{
		if (fMoveAngle != 0.f)
		{
			if (fMoveAngle > 0.f)
			{
				fMoveAngle -= 1.f;
				vecPart[3]->RotateX(-1.f);
				vecPart[4]->RotateX(1.f);
				vecPart[5]->RotateX(1.f);
				vecPart[6]->RotateX(-1.f);
				if (fMoveAngle <= 0.f)
				{
					fMoveAngle = 0.f;
				}
			}
			else
			{
				fMoveAngle += 1.f;
				vecPart[3]->RotateX(1.f);
				vecPart[4]->RotateX(-1.f);
				vecPart[5]->RotateX(-1.f);
				vecPart[6]->RotateX(1.f);
				if (fMoveAngle >= 0.f)
				{
					fMoveAngle = 0.f;
				}
			}
		}
	}

	CShape::Update();
	
	for (CCube* part : vecPart)
	{
		part->Update();
	}

	CCamera1::GetInstance()->UpdateView();
}

void CRobot::GoStraight()
{
	glm::vec3 forward = glm::normalize(glm::vec3(MatRotate[2]));
	forward.y = 0.f;
	forward = glm::normalize(forward);
	MatTranslate = glm::translate(MatTranslate, forward * fSpeed);
	if (collisionCheck())
	{
		MatTranslate = glm::translate(MatTranslate, -forward * fSpeed);
	}
}

void CRobot::BackStraight()
{
	glm::vec3 forward = glm::normalize(glm::vec3(MatRotate[2]));
	forward.y = 0.f;
	forward = glm::normalize(forward);
	MatTranslate = glm::translate(MatTranslate, -forward * fSpeed);
	if (collisionCheck())
	{
		MatTranslate = glm::translate(MatTranslate, forward * fSpeed);
	}
}

void CRobot::TurnLeft(float angle)
{
	MatRotate = glm::rotate(MatRotate, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
}

void CRobot::TurnRight(float angle)
{
	MatRotate = glm::rotate(MatRotate, glm::radians(-angle), glm::vec3(0.f, 1.f, 0.f));
}

void CRobot::SetSpeed(bool isPlus)
{
	if (isPlus)
	{
		fSpeed += 0.01f;
	}
	else
	{
		fSpeed -= 0.01f;
		if (fSpeed < 0.01f)
			fSpeed = 0.01f;
	}
}

bool CRobot::collisionCheck()
{
	glm::vec3 robotPos = glm::vec3(MatTranslate[3]);
	float disX = scaleZ + mirrorscaleX;
	float disZ = scaleZ + mirrorscaleZ;
	for (int i = 1; i < vecShape.size(); ++i)
	{
		if (!vecShape[i]->IsDel())
		{
			glm::vec3 shapePos = vecShape[i]->GetPosition() + glm::vec3(mirrorscaleX, 0.f, mirrorscaleZ);
			if (shapePos.x + disX >= robotPos.x && shapePos.x - disX <= robotPos.x)
			{
				if (shapePos.z + disZ >= robotPos.z && shapePos.z - disZ <= robotPos.z)
				{
					return true;
				}
			}
		}
	}
	return false;
}
