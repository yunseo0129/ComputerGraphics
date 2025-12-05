#include "CHand.h"
#include "Camera.h"
#include <cmath>

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
	if ((isLClicked || isRClicked) && !isSwinging)
	{
		isSwinging = true;
		iAnimFrame = 0;
		isLClicked = false;
		isRClicked = false;
	}
    if (isSwinging)
    {
        const int maxFrame = 10;  
        float t = static_cast<float>(iAnimFrame) / static_cast<float>(maxFrame);
        if (t > 1.f) t = 1.f;

        const float PI = 3.1415926535f;
        float swing = sinf(t * PI);   

        float targetRotX = swing * -35.f;

        float deltaX = targetRotX - currentRotX;
        if (std::fabs(deltaX) > 0.0001f)
        {
            HandCube->RotateX(deltaX);
            currentRotX = targetRotX;
        }

        ++iAnimFrame;
        if (iAnimFrame > maxFrame)
        {
            float back = -currentRotX;
            if (std::fabs(back) > 0.0001f)
                HandCube->RotateX(back);

            currentRotX = 0.f;
            isSwinging = false;
            iAnimFrame = 0;
        }
    }

	HandCube->Update();
}
void CHand::Render()
{
	HandCube->Draw();
}