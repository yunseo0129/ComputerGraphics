#include "SkyBox.h"

CSkyBox::CSkyBox()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
}

CSkyBox::~CSkyBox()
{
	if (bInitialized)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void CSkyBox::Initialize(glm::vec3 _pos, int _shaderID)
{
	MatTranslate = glm::translate(MatTranslate, _pos);
	MatScale = glm::scale(MatScale, glm::vec3(10.f, 10.f, 10.f));
    MatWorld = MatTranslate * MatRotate * MatScale;

    for (int i = 0; i < 6; ++i)
    {
        CCubePart* p = new CCubePart;
        p->Initialize(eCUBEPOS(i), &MatWorld, _shaderID);
		p->SetSkyBox();
        vecPart.push_back(p);
    }
}

void CSkyBox::Draw()
{
    for (CCubePart* p : vecPart)
    {
        p->Draw();
    }
}

void CSkyBox::Update()
{
    __super::Update();

    for (CCubePart* p : vecPart)
    {
        p->Update();
    }
}