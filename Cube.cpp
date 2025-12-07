#include "Cube.h"
#include "KeyMgr.h"

CCube::CCube()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
}

CCube::~CCube()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CCube::Initialize(glm::vec3 _pos, int _shaderID)
{
    MatTranslate = glm::translate(MatTranslate, _pos);
    MatWorld = MatTranslate * MatRotate * MatScale;
    for (int i = 0; i < 6; ++i)
    {
        CCubePart* p = new CCubePart;
        p->Initialize(eCUBEPOS(i), &MatWorld, _shaderID);
        vecPart.push_back(p);
    }
}

void CCube::Draw()
{
    if (scaleY <= 0.f)
        return;

    for (CCubePart* p : vecPart)
    {
        p->Draw();
    }
}

void CCube::Update()
{
	__super::Update();

    for (CCubePart* p : vecPart)
    {
        p->Update();
    }
}

void CCube::SetColor(float r, float g, float b)
{
    for (CCubePart* p : vecPart)
    {
        p->SetColor(r, g, b);
    }
}

void CCube::SetPosition(const glm::vec3& pos)
{
    MatTranslate = glm::mat4(1.0f);
    MatTranslate = glm::translate(MatTranslate, pos);
    MatWorld = MatTranslate * MatRotate * MatScale;
}
