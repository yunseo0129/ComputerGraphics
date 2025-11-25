#include "Camera1.h"

CCamera1* CCamera1::Instance = nullptr;

CCamera1::CCamera1() : vPos(12.5f, 50.0f, 32.5f), vTarget(12.5f, 0.0f, 12.5f), vUp(0.0f, 1.0f, 0.0f)
{
    UpdateView();
    UpdateProjection(800, 600);
}

CCamera1::~CCamera1()
{
}

void CCamera1::UpdateProjection(float w, float h)
{
    MatProj = glm::perspective(glm::radians(75.0f), w / h, 0.1f, 100.0f);
}

void CCamera1::UpdateView()
{
    if (robot != nullptr)
    {
        if (isFirstPerson)
        {
            vPos = robot->GetPosition();
            vPos.y = fYpos;
            vUp = glm::vec3(0.f, 1.f, 0.f);
            vTarget = vPos + glm::normalize(glm::vec3(robot->GetMatRotate()[2])) * 5.f;
            MatView = glm::lookAt(vPos, vTarget, vUp);
        }
        else
        {
            vPos = robot->GetPosition() - (glm::normalize(glm::vec3(robot->GetMatRotate()[2])) * (fYpos / 2.f));
            vPos.y = fYpos + (fYpos / 2.f);
            vUp = glm::vec3(0.f, 1.f, 0.f);
            vTarget = vPos + (glm::normalize(glm::vec3(robot->GetMatRotate()[2])) * (fYpos * 2.f));
			vTarget.y = (fYpos / 2.f);
			MatView = glm::lookAt(vPos, vTarget, vUp);
        }
    }
}
