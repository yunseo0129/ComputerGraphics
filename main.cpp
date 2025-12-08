#include "Default.h"
#include "Cube.h"
#include "Camera.h"
#include "KeyMgr.h"
#include "CPuzzle.h"
#include "CHand.h"

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void KeyInput(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void MouseInput(int button, int state, int x, int y);
void DragInput(int x, int y);
void Animation(int value);
void Initial();
void StartEnding();
void UpdateEnding();


GLint width, height;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

int WinSizeX = 800;
int WinSizeY = 600;

int level = 1;

bool gIsEnding = false;

const int END_CUBE_COUNT = 20;
CCube* gEndCubes[END_CUBE_COUNT] = {};
glm::vec3 gEndStartPos[END_CUBE_COUNT];
glm::vec3 gEndPos[END_CUBE_COUNT];
float gEndSpeed[END_CUBE_COUNT];

const int MAX_CLEAR_CUBES = 64;
CCube* gClearCubes[MAX_CLEAR_CUBES] = {};
int gClearCubeCount = 0;

CShape* gMap = nullptr;
CShape* gCreatingBox[16] = {};
CPuzzle* gPuzzle = nullptr;
CHand* gHand = nullptr;
CShape* gAim = nullptr;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinSizeX + 200, WinSizeY);
	glutCreateWindow("2019180037");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GLEW Initialized Succeced\n";

	// Shader
	make_vertexShaders();
	make_fragmentShaders();

	shaderProgramID = make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(MouseInput);
	glutMotionFunc(DragInput);

	Initial();

	glutTimerFunc(10, Animation, 51);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "“ERROR: vertex shader 컴파일 실패\n”" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint shaderID;
	GLint result;
	GLchar errorLog[512];
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}

GLvoid drawScene(GLvoid)
{
	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	// World Objects
	CCamera::GetInstance()->viewnormal();
	gPuzzle->SetView(true);
	if (gMap != nullptr)
		gMap->Draw();
	
	for (int i = 0; i < 16; ++i)
	{
		if (gCreatingBox[i] != nullptr)
			gCreatingBox[i]->Draw();
	}

	if (gPuzzle != nullptr)
		gPuzzle->Render();

	if (gHand != nullptr)
		gHand->Render();

	if (gIsEnding)
	{
		for (int i = 0; i < END_CUBE_COUNT; ++i)
		{
			if (gEndCubes[i] != nullptr)
				gEndCubes[i]->Draw();
		}
		for (int i = 0; i < gClearCubeCount; ++i)
		{
			if (gClearCubes[i] != nullptr)
				gClearCubes[i]->Draw();
		}
	}

	// UI
	CCamera::GetInstance()->viewui();
	if (gAim != nullptr)
		gAim->Draw();

	gPuzzle->SetView(false);
	CCamera::GetInstance()->viewfront();
	glViewport(805, 407, 190, 190);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (gPuzzle != nullptr)
		gPuzzle->Render();
	CCamera::GetInstance()->viewside();
	glViewport(805, 207, 190, 190);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (gPuzzle != nullptr)
		gPuzzle->Render();
	CCamera::GetInstance()->viewup();
	glViewport(805, 7, 190, 190);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (gPuzzle != nullptr)
		gPuzzle->Render();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	WinSizeX = w;
	WinSizeY = h;
	glutPostRedisplay();
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		if (gPuzzle->AnswerCheck())
		{
			if (++level == 4)
			{
				gPuzzle->LevelSet(level);
				StartEnding();
			}
			else
				gPuzzle->LevelSet(level);
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		return;
	default:
		break;
	}
	
	glutPostRedisplay();
}

void KeyUp(unsigned char key, int x, int y)
{

}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gPuzzle->Click(true);
		gHand->SetLClicked(true);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gPuzzle->Click(false);
		gHand->SetLClicked(true);
	}
	glutPostRedisplay();
}

void DragInput(int x, int y)
{
	
}

void Animation(int value)
{
	CKeyMgr::Get_Instance()->Key_Update();
	CCamera::GetInstance()->UpdateView();

	gPuzzle->Update();
	gHand->Update();

	if (gIsEnding)
	{
		UpdateEnding();
	}

	glutPostRedisplay();
	glutTimerFunc(10, Animation, value);
}

void Initial()
{
	CCamera::GetInstance()->Initial();
	while (ShowCursor(FALSE) >= 0);

	gMap = new CCube();
	gMap->Initialize(glm::vec3(-6.f, -1.f, -6.f), shaderProgramID);
	gMap->SetScale(16.f, 1.f, 16.f);
	static_cast<CCube*>(gMap)->SetColor(0.3f, 0.9f, 0.6f);
	gMap->Update();

	for (int i = 0; i < 16; ++i)
	{
		gCreatingBox[i] = new CCube();
		gCreatingBox[i]->Initialize(glm::vec3((float)(i / 4), -0.999f, (float)(i % 4)), shaderProgramID);
		gCreatingBox[i]->SetScale(1.f, 1.f, 1.f);
		static_cast<CCube*>(gCreatingBox[i])->SetColor(1.f, 0.2f, 0.2f);
		gCreatingBox[i]->Update();
	}

	gPuzzle = new CPuzzle();
	gPuzzle->Initialize(shaderProgramID);
	gPuzzle->LevelSet(level);

	gHand = new CHand();
	gHand->Initialize(shaderProgramID);

	gAim = new CCube();
	gAim->Initialize(glm::vec3(0.f, 0.f, 0.f), shaderProgramID);
	gAim->SetScale(0.05f, 0.05f, 0.05f);
	static_cast<CCube*>(gAim)->SetColor(1.f, 1.f, 1.f);
	gAim->Update();
}

void StartEnding()
{
	gIsEnding = true;

	for (int i = 0; i < END_CUBE_COUNT; ++i)
	{
		gEndCubes[i] = new CCube();

		float startX = -4.0f + RandFloat0_1() * 8.0f;
		float startZ = -4.0f + RandFloat0_1() * 8.0f;
		float startY = 3.0f + RandFloat0_1() * 4.0f;

		gEndStartPos[i] = glm::vec3(startX, startY, startZ);
		gEndPos[i] = gEndStartPos[i];
		gEndSpeed[i] = 0.02f + RandFloat0_1() * 0.03f;

		gEndCubes[i]->Initialize(gEndPos[i], shaderProgramID);
		gEndCubes[i]->SetScale(0.15f, 0.15f, 0.15f);
		gEndCubes[i]->SetColor(1.0f, 1.0f, 1.0f);
		gEndCubes[i]->Update();
	}
	gClearCubeCount = 0;

	float cell = 0.4f;
	float baseY = 0.5f;
	float startX = -2.0f;
	float wallZ = 2.0f;

	const char* C_pattern[5] =
	{
		"XXX",
		"X..",
		"X..",
		"X..",
		"XXX"
	};
	const char* L_pattern[5] =
	{
		"X..",
		"X..",
		"X..",
		"X..",
		"XXX"
	};
	const char* E_pattern[5] =
	{
		"XXX",
		"X..",
		"XXX",
		"X..",
		"XXX"
	};
	const char* A_pattern[5] =
	{
		".X.",
		"X.X",
		"XXX",
		"X.X",
		"X.X"
	};
	const char* R_pattern[5] =
	{
		"XX.",
		"X.X",
		"XX.",
		"X.X",
		"X.X"
	};
	const char** patterns[5] = { C_pattern, L_pattern, E_pattern, A_pattern, R_pattern };

	for (int letter = 0; letter < 5; ++letter)
	{
		float letterOffsetX = startX + (4 - letter) * (3 * cell + 0.4f);

		for (int row = 0; row < 5; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				if (patterns[letter][row][col] == 'X')
				{
					if (gClearCubeCount >= MAX_CLEAR_CUBES)
						break;

					int rowIdx = 4 - row;  
					int colIdx = 2 - col; 

					float x = letterOffsetX + colIdx * cell;
					float y = baseY + rowIdx * cell;
					float z = wallZ;

					gClearCubes[gClearCubeCount] = new CCube();
					gClearCubes[gClearCubeCount]->Initialize(glm::vec3(x, y, z), shaderProgramID);
					gClearCubes[gClearCubeCount]->SetScale(0.25f, 0.25f, 0.25f);
					gClearCubes[gClearCubeCount]->SetColor(1.0f, 0.2f, 0.2f);
					gClearCubes[gClearCubeCount]->Update();

					++gClearCubeCount;
				}
			}
		}
	}
}


void UpdateEnding()
{
	for (int i = 0; i < END_CUBE_COUNT; ++i)
	{
		if (!gEndCubes[i]) continue;

		gEndPos[i].y -= gEndSpeed[i];

		if (gEndPos[i].y <= -1.0f)
			gEndPos[i].y = gEndStartPos[i].y;

		gEndCubes[i]->SetPosition(gEndPos[i]);
		gEndCubes[i]->Update();
	}
}