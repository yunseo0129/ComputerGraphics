#include "Line.h"
#include "Camera.h"

CLine::CLine()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
    MatWorld = glm::mat4(1.f);
}

CLine::~CLine()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CLine::Initialize(glm::vec3 _pos, int _shaderID)
{
    shaderProgramID = _shaderID;
    MatWorld = glm::translate(MatWorld, _pos);
}

void CLine::Draw()
{
    GLfloat vertices[] = {
       vPoint[0].x, vPoint[0].y, vPoint[0].z,
       vPoint[1].x, vPoint[1].y, vPoint[1].z
    };

    if (!bInitialized) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        bInitialized = true;
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    }

    glUseProgram(shaderProgramID);

    glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * MatWorld;
    GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

    GLint locColor = glGetUniformLocation(shaderProgramID, "uColor");
    glUniform3f(locColor, color.r, color.g, color.b);

    glBindVertexArray(VAO);
    glLineWidth(0.5f);                 // 선 굵기
    glDrawArrays(GL_LINES, 0, 2);      // 두 점으로 선 그리기
    glBindVertexArray(0);
}

void CLine::Update()
{
  
}

void CLine::SetPoint(glm::vec4 p1, glm::vec4 p2)
{
    vPoint[0] = p1;
    vPoint[1] = p2;
}
