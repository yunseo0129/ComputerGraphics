#include "CPyramidPart.h"
#include "Camera.h"

CPyramidPart::CPyramidPart()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
    MatWorld = glm::mat4(1.f);
}

CPyramidPart::~CPyramidPart()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CPyramidPart::Initialize(eCUBEPOS _pos, glm::mat4* _parent, int _shaderID)
{
    shaderProgramID = _shaderID;
    MatParent = _parent;

    switch (_pos)
    {
    case PLUS_X: // +X 면
        vPoint[0].pos = glm::vec4(0.f, 1.f, 0.f, 1.f); // top-left
        vPoint[1].pos = glm::vec4(1.f, -1.f, 1.f, 1.f); // top-right
        vPoint[2].pos = glm::vec4(1.f, -1.f, -1.f, 1.f); // bottom-right
        break;
    case MINUS_X: // -X 면
        vPoint[0].pos = glm::vec4(0.f, 1.f, 0.f, 1.f); 
        vPoint[1].pos = glm::vec4(-1.f, -1.f, -1.f, 1.f);
        vPoint[2].pos = glm::vec4(-1.f, -1.f, 1.f, 1.f);
        break;
    case MINUS_Y: // -Y 면
        isBottom = true;
        vPoint[0].pos = glm::vec4(-1.f, -1.f, 1.f, 1.f);
        vPoint[1].pos = glm::vec4(-1.f, -1.f, -1.f, 1.f);
        vPoint[2].pos = glm::vec4(1.f, -1.f, -1.f, 1.f);
        vPoint[3].pos = glm::vec4(1.f, -1.f, 1.f, 1.f);
        break;
    case PLUS_Z: // +Z 면
        vPoint[0].pos = glm::vec4(0.f, 1.f, 0.f, 1.f); 
        vPoint[1].pos = glm::vec4(-1.f, -1.f, 1.f, 1.f);
        vPoint[2].pos = glm::vec4(1.f, -1.f, 1.f, 1.f);
        break;
    case MINUS_Z: // -Z 면
        vPoint[0].pos = glm::vec4(0.f, 1.f, 0.f, 1.f); 
        vPoint[1].pos = glm::vec4(1.f, -1.f, -1.f, 1.f);
        vPoint[2].pos = glm::vec4(-1.f, -1.f, -1.f, 1.f);
        break;
    default:
        break;
    }
}

void CPyramidPart::Draw()
{
    if (isBottom)
    {
        GLuint indices[] = { 0,1,2, 0, 2, 3 };

        if (!bInitialized) {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(VerTex) * 4, vPoint, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 위치 속성 (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)offsetof(VerTex, color));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            bInitialized = true;
        }
        else {
            // 이미 초기화 됐으면 버퍼 업데이트
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VerTex) * 4, vPoint);
        }

        // 3. 그리기
        glUseProgram(shaderProgramID);

        glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * *MatParent * MatWorld;
        GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
    else
    {
        GLuint indices[] = { 0,1,2 };

        if (!bInitialized) {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(VerTex) * 3, vPoint, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 위치 속성 (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerTex), (void*)offsetof(VerTex, color));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            bInitialized = true;
        }
        else {
            // 이미 초기화 됐으면 버퍼 업데이트
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VerTex) * 3, vPoint);
        }

        // 3. 그리기
        glUseProgram(shaderProgramID);

        glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * *MatParent * MatWorld;
        GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}

void CPyramidPart::Update()
{

}