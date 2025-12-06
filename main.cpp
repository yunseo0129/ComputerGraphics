#include "Default.h"
#include "Cube.h"
#include "Camera.h"
#include "KeyMgr.h"
#include "CLight.h"
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

GLint width, height;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

int WinSizeX = 800;
int WinSizeY = 600;

int level = 1;

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

	//CLight::GetInstance()->ApplyAmbient(shaderProgramID);
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

	// UI
	CCamera::GetInstance()->viewui();
	if (gAim != nullptr)
		gAim->Draw();

	gPuzzle->SetView(false);
	CCamera::GetInstance()->viewfront();
	glViewport(800, 400, 200, 200);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (gPuzzle != nullptr)
		gPuzzle->Render();
	CCamera::GetInstance()->viewside();
	glViewport(800, 200, 200, 200);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (gPuzzle != nullptr)
		gPuzzle->Render();
	CCamera::GetInstance()->viewup();
	glViewport(800, 0, 200, 200);
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
	case 'm':  
		CLight::GetInstance()->ToggleLight();
		break;
	case '+':  
		CLight::GetInstance()->SetAmbientStrength(0.6f);
		break;
	case '-':  
		CLight::GetInstance()->SetAmbientStrength(0.1f);
		break;
	case 'r':
		if (gPuzzle->AnswerCheck())
			gPuzzle->LevelSet(++level);
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

	glutPostRedisplay();
	glutTimerFunc(10, Animation, value);
}

void Initial()
{
	CCamera::GetInstance()->Initial();
	CLight::GetInstance()->Init();
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