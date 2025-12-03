#include "Camera.h"
#include "KeyMgr.h"

CCamera* CCamera::Instance = nullptr;

CCamera::CCamera()
{
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
    KeyInput();

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    vLook = glm::normalize(front);

    vRight = glm::normalize(glm::cross(vLook, glm::vec3(0.f, 1.f, 0.f)));
    vUp = glm::normalize(glm::cross(vRight, vLook));

	MatView = glm::lookAt(vPos, vPos + vLook, vUp);
}

void CCamera::Initial()
{
    UpdateProjection(800, 600);
    UpdateView();
	MatProjOrthor = glm::ortho(-4.f, 4.f, -3.f, 3.f, 0.1f, 100.f);
	MatViewOrthor = glm::lookAt(glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f));
}

glm::mat4 CCamera::GetMatProjView() const
{
    if (UIMode)
		return MatProjOrthor * MatViewOrthor;
    return MatProj * MatView;
}

bool CCamera::CheckCollision(const glm::vec3& nextPos)
{

}

void CCamera::KeyInput()
{
    // 키보드 입력
	glm::vec3 front;
	front.x = vLook.x;
	front.y = 0.f;
	front.z = vLook.z;
	front = glm::normalize(front);
	glm::vec3 nextPos = vPos;

    if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		nextPos += cameraSpeed * front;
    if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		nextPos -= cameraSpeed * front;
    if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		nextPos -= glm::normalize(glm::cross(front, vUp)) * cameraSpeed;
    if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		nextPos += glm::normalize(glm::cross(front, vUp)) * cameraSpeed;
    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		nextPos += cameraSpeed * glm::vec3(0.f, 1.f, 0.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LSHIFT))
	{
		nextPos -= cameraSpeed * glm::vec3(0.f, 1.f, 0.f);
		if (nextPos.y < 1.7f)
			nextPos.y = 1.7f;
	}

	if(!CheckCollision(nextPos))
		vPos = nextPos;

    // 마우스 입력
	POINT currMousePos;
	GetCursorPos(&currMousePos);
    SetCursorPos(CenterPoint.x, CenterPoint.y);
	float xoffset = currMousePos.x - CenterPoint.x;
	float yoffset = CenterPoint.y - currMousePos.y;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	// 피치 제한
    if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}
