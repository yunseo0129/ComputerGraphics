#pragma once
#include "Shape.h"

class CCircle : public CShape
{
public:
    CCircle(float _radius = 1.f, int _segments = 100, float _rotZ = 0.f);
    ~CCircle();

    void Initialize(glm::vec3 _pos, int _shaderID) override;
    void Draw() override;
    void Update() override;

    void SetRotationZ(float deg) { rotZ = deg; }

private:
    GLuint VAO, VBO;
    std::vector<VerTex> verts;

    int shaderProgramID;
    float radius;
    int segments;
    float rotZ; // ZÃà È¸Àü
};