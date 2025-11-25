#include "Pyramid.h"
#include "KeyMgr.h"

CPyramid::CPyramid()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
}

CPyramid::~CPyramid()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CPyramid::Initialize(glm::vec3 _pos, int _shaderID)
{
    MatTranslate = glm::translate(MatTranslate, _pos);
    MatWorld = MatTranslate * MatRotate * MatScale;

    for (int i = 0; i < 6; ++i)
    {
        CPyramidPart* p = new CPyramidPart;
        p->Initialize(eCUBEPOS(i), &MatWorld, _shaderID);
        vecPart.push_back(p);
    }
}

void CPyramid::Draw()
{
    for (CPyramidPart* p : vecPart)
    {
        p->Draw();
    }
}

void CPyramid::Update()
{
	__super::Update();

    for (CPyramidPart* p : vecPart)
    {
        p->Update();
    }
}