#include "Default.h"
#include "Cube.h"
#include "Camera.h"
#include "KeyMgr.h"

// 윤서 브렌치 테스트

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

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinSizeX, WinSizeY);
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
	glClearColor(0.f, 0.f, 0.f, 0.f);

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
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
	GLclampf MouseX = (x / GLfloat(WinSizeX / 2)) - 1.f;
	GLclampf MouseY = -((y / GLfloat(WinSizeY / 2)) - 1.f);
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	glutPostRedisplay();
}

void DragInput(int x, int y)
{
	
}

void Animation(int value)
{
	CCamera::GetInstance()->UpdateView();
	CKeyMgr::Get_Instance()->Key_Update();

	glutPostRedisplay();
	glutTimerFunc(10, Animation, value);
}

void Initial()
{
	CCamera::GetInstance()->Initial();
}