#include "Circle.h"
#include "Camera.h"
CCircle::CCircle(float _radius, int _segments, float _rotZ)
{
    VAO = VBO = 0;
    radius = _radius;
    segments = _segments;
    rotZ = _rotZ;
    bInitialized = false;
}

CCircle::~CCircle()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CCircle::Initialize(glm::vec3 _pos, int _shaderID)
{
    shaderProgramID = _shaderID;
    MatTranslate = glm::translate(glm::mat4(1.f), _pos);

    verts.clear();
    verts.push_back({ glm::vec3(0.f), glm::vec3(1.f,0.5f,1.f) }); // center

    glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), glm::radians(rotZ), glm::vec3(0.f, 1.f, 0.f));

    for (int i = 0; i <= segments; i++)
    {
        float angle = i * 2.f * 3.1415926f / segments;
        glm::vec4 pos(radius * cos(angle), 0.f, radius * sin(angle), 1.f);
        pos = rotMat * pos;
        verts.push_back({ glm::vec3(pos), glm::vec3(1.f,0.5f,1.f) });
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VerTex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)offsetof(VerTex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    bInitialized = true;
}

void CCircle::Draw()
{
    glUseProgram(shaderProgramID);
    glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * MatTranslate;
    GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size());
    glBindVertexArray(0);
}

void CCircle::Update()
{
    // 필요하면 여기에 회전 애니메이션 추가
}