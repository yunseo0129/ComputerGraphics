#include "CHand.h"
#include "Camera.h"

CHand::CHand()
{
}
CHand::~CHand()
{
	delete HandCube;
}
void CHand::Initialize(int _shaderID)
{
	HandCube = new CCube;
	HandCube->Initialize(glm::vec3(0.16f, -0.3f, -0.3f), _shaderID);
	HandCube->SetScale(0.1f, 0.3f, 0.1f);
	HandCube->RotateX(-60.f);
	HandCube->RotateY(30.f);
	HandCube->RotateZ(15.f);
	//HandCube->SetColor(1.f, 1.f, 0.f);
	HandCube->SetParentMat(CCamera::GetInstance()->GetWorldMatPtr());
}
void CHand::Update()
{
	if (isLClicked)
	{
		switch (iAnimFrame)
		{
		case 0:
			++iAnimFrame;
			break;
		default:
			break;
		}
	}
	else if (isRClicked)
	{
		switch (iAnimFrame)
		{
		case 0:
			++iAnimFrame;
			break;
		default:
			break;
		}
	}

	HandCube->Update();
}
void CHand::Render()
{
	HandCube->Draw();
}