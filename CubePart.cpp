#include "CubePart.h"
#include "Camera.h"
#include "Camera1.h"

CCubePart::CCubePart()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    bInitialized = 0;
    MatWorld = glm::mat4(1.f);
}

CCubePart::~CCubePart()
{
    if (bInitialized) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void CCubePart::Initialize(eCUBEPOS _pos, glm::mat4* _parent, int _shaderID)
{
    shaderProgramID = _shaderID;
    MatParent = _parent;
    if (isMap)
    {
        glm::vec4 p000 = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 p100 = glm::vec4(1.f, 0.f, 0.f, 1.f);
        glm::vec4 p010 = glm::vec4(0.f, 1.f, 0.f, 1.f);
        glm::vec4 p001 = glm::vec4(0.f, 0.f, 1.f, 1.f);
        glm::vec4 p110 = glm::vec4(1.f, 1.f, 0.f, 1.f);
        glm::vec4 p011 = glm::vec4(0.f, 1.f, 1.f, 1.f);
        glm::vec4 p101 = glm::vec4(1.f, 0.f, 1.f, 1.f);
        glm::vec4 p111 = glm::vec4(1.f, 1.f, 1.f, 1.f);
        switch (_pos)
        {
        case PLUS_X: // +X 면
            vPoint[0].pos = p110; // top-left
            vPoint[1].pos = p111; // top-right
            vPoint[2].pos = p101; // bottom-right
            vPoint[3].pos = p100; // bottom-left
            break;
        case MINUS_X: // -X 
            vPoint[0].pos = p011;
            vPoint[1].pos = p010;
            vPoint[2].pos = p000;
            vPoint[3].pos = p001;
            break;
        case PLUS_Y: // +Y
            vPoint[0].pos = p011;
            vPoint[1].pos = p111;
            vPoint[2].pos = p110;
            vPoint[3].pos = p010;
            break;
        case MINUS_Y: // -Y
            vPoint[0].pos = p000;
            vPoint[1].pos = p100;
            vPoint[2].pos = p101;
            vPoint[3].pos = p001;
            break;
        case PLUS_Z: // +Z 
            vPoint[0].pos = p010;
            vPoint[1].pos = p110;
            vPoint[2].pos = p100;
            vPoint[3].pos = p000;
            break;
        case MINUS_Z: // -Z 
            vPoint[0].pos = p111;
            vPoint[1].pos = p011;
            vPoint[2].pos = p001;
            vPoint[3].pos = p101;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (_pos)
        {
        case PLUS_X: // +X 면
            vPoint[0].pos = glm::vec4(1.f, 2.f, -1.f, 1.f); // top-left
            vPoint[1].pos = glm::vec4(1.f, 2.f, 1.f, 1.f); // top-right
            vPoint[2].pos = glm::vec4(1.f, 0.f, 1.f, 1.f); // bottom-right
            vPoint[3].pos = glm::vec4(1.f, 0.f, -1.f, 1.f); // bottom-left
            break;
        case MINUS_X: // -X 면
            vPoint[0].pos = glm::vec4(-1.f, 2.f, 1.f, 1.f);
            vPoint[1].pos = glm::vec4(-1.f, 2.f, -1.f, 1.f);
            vPoint[2].pos = glm::vec4(-1.f, 0.f, -1.f, 1.f);
            vPoint[3].pos = glm::vec4(-1.f, 0.f, 1.f, 1.f);
            break;
        case PLUS_Y: // +Y 면
            vPoint[0].pos = glm::vec4(-1.f, 2.f, -1.f, 1.f);
            vPoint[1].pos = glm::vec4(-1.f, 2.f, 1.f, 1.f);
            vPoint[2].pos = glm::vec4(1.f, 2.f, 1.f, 1.f);
            vPoint[3].pos = glm::vec4(1.f, 2.f, -1.f, 1.f);
            break;
        case MINUS_Y: // -Y 면
            vPoint[0].pos = glm::vec4(-1.f, 0.f, 1.f, 1.f);
            vPoint[1].pos = glm::vec4(-1.f, 0.f, -1.f, 1.f);
            vPoint[2].pos = glm::vec4(1.f, 0.f, -1.f, 1.f);
            vPoint[3].pos = glm::vec4(1.f, 0.f, 1.f, 1.f);
            break;
        case PLUS_Z: // +Z 면
            vPoint[0].pos = glm::vec4(1.f, 2.f, 1.f, 1.f);
            vPoint[1].pos = glm::vec4(-1.f, 2.f, 1.f, 1.f);
            vPoint[2].pos = glm::vec4(-1.f, 0.f, 1.f, 1.f);
            vPoint[3].pos = glm::vec4(1.f, 0.f, 1.f, 1.f);
            break;
        case MINUS_Z: // -Z 면
            vPoint[0].pos = glm::vec4(-1.f, 2.f, -1.f, 1.f);
            vPoint[1].pos = glm::vec4(1.f, 2.f, -1.f, 1.f);
            vPoint[2].pos = glm::vec4(1.f, 0.f, -1.f, 1.f);
            vPoint[3].pos = glm::vec4(-1.f, 0.f, -1.f, 1.f);
            break;
        default:
            break;
        }
    }
	glm::vec3 color = glm::vec3(RandFloat0_1(), RandFloat0_1(), RandFloat0_1());
    for (int i = 0; i < 4; ++i)
    {
        vPoint[i].color = color;
	}
}

void CCubePart::Draw()
{
    GLuint indices[] = { 0,1,2, 0, 2, 3 };
    if (isSkyBox)
    {
        indices[0] = 2;
        indices[1] = 1;
        indices[2] = 0;
        indices[3] = 3;
        indices[4] = 2;
        indices[5] = 0;
	}

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
    glm::mat4 MVP;
    if (CCamera::GetInstance()->GetMini())
		MVP = CCamera::GetInstance()->GetMatProjView() * *MatParent * MatWorld;
    else if (CCamera1::GetInstance()->GetEnable())
        MVP = CCamera1::GetInstance()->GetMatProjView() * *MatParent * MatWorld;
    else
        MVP = CCamera::GetInstance()->GetMatProjView() * *MatParent * MatWorld;

    GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void CCubePart::Update()
{
    
}

void CCubePart::SetColor(float x, float y, float z)
{
    for (int i = 0; i < 4; ++i)
    {
        vPoint[i].color = glm::vec3(x, y, z);
    }
}
