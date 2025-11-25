#include "Camera.h"

CCamera* CCamera::Instance = nullptr;

CCamera::CCamera() : vPos(12.5f, 50.0f, 32.5f), vTarget(12.5f, 0.0f, 12.5f), vUp(0.0f, 1.0f, 0.0f)
{
    UpdateView();
    UpdateProjection(800, 600);
}

CCamera::~CCamera()
{
}

void CCamera::UpdateProjection(float w, float h)
{
    MatProj = glm::perspective(glm::radians(45.0f), w / h, 0.1f, 100.0f);
}

void CCamera::UpdateView()
{
    MatView = glm::lookAt(vPos, vTarget, vUp);
}

void CCamera::Initial()
{
    vPos = glm::vec3(12.5f, 50.0f, 32.5f);
    vTarget = glm::vec3(12.5f, 0.0f, 12.5f);
    vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateView();

    MatMiniProj = glm::ortho(-12.5f, 12.5f, -12.5f, 12.5f, 0.1f, 500.f);
    MatMiniView = glm::lookAt(
        glm::vec3(12.5f, 50.f, 12.5f),
        glm::vec3(12.5f, 0.f, 12.5f),
        glm::vec3(0.f, 0.f, -1.f)
    );
}

glm::mat4 CCamera::GetMatProjView() const
{
    if (isMini)
        return MatMiniProj * MatMiniView;
    else
        return MatProj * MatView;
}
