#include "Obj.h"
#include "Camera.h"

CObj::CObj()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	bInitialized = 0;
}

CObj::~CObj()
{
	if (bInitialized) {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
}

void CObj::Initialize(glm::vec3 _pos, int _shaderID)
{
	shaderProgramID = _shaderID;

	MatTranslate = glm::translate(MatTranslate, _pos);
	MatWorld = MatTranslate * MatRotate * MatScale;
}

void CObj::Draw()
{
	if (!bInitialized) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VerTex) * iVertexCount, pVertex, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * iFaceCount, pIndices, GL_STATIC_DRAW);

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
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VerTex) * iVertexCount, pVertex);
	}

	// 3. 그리기
	glUseProgram(shaderProgramID);

	glm::mat4 MVP = CCamera::GetInstance()->GetMatProjView() * MatWorld;
	GLuint locMVP = glGetUniformLocation(shaderProgramID, "MVP");
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));
	glm::vec4 clip = MVP * glm::vec4(pVertex[0].pos, 1.0f);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3 * iFaceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void CObj::Update()
{
	if (isCenter)
	{
		MatRotate = glm::rotate(MatRotate, glm::radians(-2.f), glm::vec3(0.f, 1.f, 0.f));
	}
	else if (isSmall)
	{
		MatParentRot = glm::rotate(MatParentRot, glm::radians(2.5f), glm::vec3(0.f, 1.f, 0.f));
	}

	if (MatOrbit == nullptr)
		MatWorld = MatTranslate * MatRotate * MatScale;
	else if (isSmall)
		MatWorld =  *MatEarth * *MatOrbit *MatParentRot * MatTranslate * MatRotate * MatScale;
	else
		MatWorld = *MatOrbit * MatTranslate * MatRotate * MatScale;
}

void CObj::SetObj(const char* _objFile)
{
	VerTex t1, t2, t3;
	t1.color = glm::vec3(RandFloat(0.1f, 0.9f), RandFloat(0.1f, 0.9f), RandFloat(0.1f, 0.9f));
	t2.color = t1.color - glm::vec3(0.1f, 0.1f, 0.1f);
	t3.color = t1.color + glm::vec3(0.1f, 0.1f, 0.1f);
	if (!strcmp("Heart.obj", _objFile))
	{
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(0.05f, 0.05f, 0.05f));
	}
	else if (!strcmp("Star.obj", _objFile))
	{
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(8.f, 8.f, 8.f));
		MatRotate = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	}
	else if (!strcmp("Sphere.obj", _objFile))
	{
		MatScale = glm::scale(glm::mat4(1.f), glm::vec3(5.f, 5.f, 5.f));
	}
	FILE* file;
	fopen_s(&file, _objFile, "r");
	if (file == NULL)
	{
		cout << "Failed to open the file!" << endl;
		return;
	}
	char line[128];
	while (fgets(line, sizeof(line), file))
	{
		read_newline(line);

		if (line[0] == 'v' && line[1] == ' ')
			iVertexCount++;
		else if (line[0] == 'f' && line[1] == ' ')
			iFaceCount++;
	}
	fseek(file, 0, SEEK_SET);
	if (!strcmp("Sphere.obj", _objFile))
	{
		iFaceCount *= 2;
	}
	pVertex = (VerTex*)malloc(iVertexCount * sizeof(VerTex));
	pIndices = (Face*)malloc(iFaceCount * sizeof(Face));
	size_t vertex_index = 0;    size_t face_index = 0;
	while (fgets(line, sizeof(line), file)) {
		read_newline(line);
		if (line[0] == 'v' && line[1] == ' ') {
			int result = sscanf_s(line + 2, "%f %f %f", &pVertex[vertex_index].pos.x,
				&pVertex[vertex_index].pos.y,
				&pVertex[vertex_index].pos.z);
			if (!strcmp("Heart.obj", _objFile))
			{
				switch ((int)vertex_index % 3)
				{
				case 0:
					pVertex[vertex_index].color = glm::vec3(1.0f, 0.3f, 0.5f);
					break;
				case 1:
					pVertex[vertex_index].color = glm::vec3(1.0f, 0.4f, 0.6f);
					break;
				case 2:
					pVertex[vertex_index].color = glm::vec3(1.0f, 0.5f, 0.7f);
					break;
				default:
					break;
				}
			}
			else if (!strcmp("Star.obj", _objFile))
			{
				switch ((int)vertex_index % 3)
				{
				case 0:
					pVertex[vertex_index].color = glm::vec3(1.0f, 0.9f, 0.2f);
					break;
				case 1:
					pVertex[vertex_index].color = glm::vec3(1.0f, 0.95f, 0.4f);
					break;
				case 2:
					pVertex[vertex_index].color = glm::vec3(1.0f, 1.0f, 0.6f);
					break;
				default:
					break;
				}
			}
			else if (!strcmp("Sphere.obj", _objFile))
			{
				pVertex[vertex_index].color = glm::vec3(RandFloat0_1(), RandFloat0_1(), RandFloat0_1());
			}
			vertex_index++;
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			if (strcmp("Sphere.obj", _objFile) == 0)
			{
				unsigned int v[4];
				int count = 0;
				char* p = line + 2; // 'f ' 다음부터
				for (int i = 0; i < 4; ++i)
				{
					while (*p == ' ') ++p; // 공백 건너뛰기
					// 슬래시가 나오면 잘라서 정수만 읽기
					v[i] = strtoul(p, &p, 10); // 정점 번호 읽기
					while (*p && *p != ' ') ++p; // 다음 공백까지 이동
				}
				// 삼각형 2개로 분할
				pIndices[face_index].v1 = v[0] - 1;
				pIndices[face_index].v2 = v[1] - 1;
				pIndices[face_index].v3 = v[2] - 1;
				face_index++;
				pIndices[face_index].v1 = v[0] - 1;
				pIndices[face_index].v2 = v[2] - 1;
				pIndices[face_index].v3 = v[3] - 1;
				face_index++;
			}
			else
			{
				unsigned int v[3];
				int count = 0;

				char* p = line + 2; // 'f ' 다음부터

				for (int i = 0; i < 3; ++i)
				{
					while (*p == ' ') ++p; // 공백 건너뛰기

					// 슬래시가 나오면 잘라서 정수만 읽기
					v[i] = strtoul(p, &p, 10); // 정점 번호 읽기
					while (*p && *p != ' ') ++p; // 다음 공백까지 이동
				}

				pIndices[face_index].v1 = v[0] - 1;
				pIndices[face_index].v2 = v[1] - 1;
				pIndices[face_index].v3 = v[2] - 1;
				face_index++;
			}
		}
	}
	fclose(file);
}
